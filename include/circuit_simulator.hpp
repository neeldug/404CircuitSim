#ifndef GUARD_CIRCUIT_SIMULATOR_HPP
#define GUARD_CIRCUIT_SIMULATOR_HPP
#include <sstream>
#include <iostream>
class Circuit::Simulator
{
private:
    Schematic *schem;
    double tranStopTime;
    double tranSaveStart;
    double tranStepTime;
    std::stringstream spiceStream;
    std::stringstream csvStream;

    double errorThreshold = 1e-10;

    void spicePrintTitle()
    {
        spiceStream << "Time\t";
        for (auto node_pair : schem->nodes)
        {
            spiceStream << "V(" << node_pair.first << ")\t";
        }
        for (auto comp_pair : schem->comps)
        {
            spiceStream << "I(" << comp_pair.first << ")\t";
        }
        spiceStream << "\n";
    }
    void csvPrintTitle()
    {
        csvStream << "Time,";
        for (auto node_pair : schem->nodes)
        {
            csvStream << "V(" << node_pair.first << "),";
        }
        for (auto comp_pair : schem->comps)
        {
            csvStream << "I(" << comp_pair.first << "),";
        }
        csvStream << "\n";
    }
    void spicePrint(ParamTable *param, double time, double timestep)
    {
        spiceStream << time << "\t";
        for (auto node_pair : schem->nodes)
        {
            spiceStream << node_pair.second->voltage << "\t";
        }
        for (auto comp_pair : schem->comps)
        {
            spiceStream << comp_pair.second->getCurrent(param, time, timestep) << "\t";
        }
        spiceStream << "\n";
    }
    void csvPrint(ParamTable *param, double time, double timestep)
    {
        csvStream << time << ",";
        for (auto node_pair : schem->nodes)
        {
            csvStream << node_pair.second->voltage << ",";
        }
        for (auto comp_pair : schem->comps)
        {
            csvStream << comp_pair.second->getCurrent(param, time, timestep) << ",";
        }
        csvStream << "\n";
    }

public:
    enum SimulationType
    {
        OP,
        TRAN,
        DC,
        SMALL_SIGNAL
    };

    enum OutputFormat
    {
        CSV,
        SPACE // actually tab separated
    };

    using enumPair = std::pair<SimulationType, std::string>;

    std::map<SimulationType, std::string> simulationTypeMap = {
        enumPair(OP, "OP"),
        enumPair(TRAN, "TRAN"),
        enumPair(DC, "DC"),
        enumPair(SMALL_SIGNAL, "SMALL_SIGNAL"),
    };

    const SimulationType type;

    Simulator(Schematic *schem, SimulationType type) : type(type), schem(schem) {}
    Simulator(Schematic *schem, SimulationType type, double tranStopTime, double tranSaveStart = 0, double tranStepTime = 0) : Simulator(schem, type)
    {
        if (tranStepTime == 0)
        {
            tranStepTime = tranStopTime / 1000.0; // default of a thousand cycles
        }
        this->tranStopTime = tranStopTime;
        this->tranSaveStart = tranSaveStart;
        this->tranStepTime = tranStepTime;
    }

    void run(std::ostream &dst, OutputFormat format)
    {
        for (ParamTable *param : schem->tables)
        {
            if (type == OP)
            {
                const int NUM_NODES = schem->nodes.size() - 1;
                Eigen::VectorXd voltage(NUM_NODES);
                Eigen::VectorXd current(NUM_NODES);
                Eigen::MatrixXd conductance(NUM_NODES, NUM_NODES);
                Eigen::SparseLU<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int>> solver;

                Circuit::Math::getConductanceOP(schem, conductance, param);
                Circuit::Math::getCurrentOP(schem, current, conductance, param);

                Eigen::SparseMatrix<double> sparse = conductance.sparseView();
                sparse.makeCompressed();
                solver.analyzePattern(sparse);
                solver.factorize(sparse);
                voltage = solver.solve(current);

                dst << "\t-----Operating Point-----\t\n\n";

                for_each(schem->nodes.begin(), schem->nodes.end(), [&](const auto node_pair) {
                    if (node_pair.second->getId() != -1)
                    {
                        node_pair.second->voltage = voltage[node_pair.second->getId()];
                        dst << "V(" << node_pair.first << ")\t\t" << node_pair.second->voltage << "\tnode_voltage\n";
                    }
                });

                for_each(schem->comps.begin(), schem->comps.end(), [&](const auto comp_pair) {
                    dst << "I(" << comp_pair.first << ")\t\t" << comp_pair.second->getCurrent(param, 0, -1) << "\tdevice_current\n";
                });
            }
            else if (type == TRAN)
            {
                if (format == SPACE)
                {
                    spiceStream.str("");
                    spicePrintTitle();
                }
                else if (format == CSV)
                {
                    csvStream.str("");
                    csvPrintTitle();
                }

                if (!schem->nonLinear)
                {

                    const int NUM_NODES = schem->nodes.size() - 1;
                    Eigen::VectorXd voltage(NUM_NODES);
                    Eigen::VectorXd current(NUM_NODES);
                    Eigen::MatrixXd conductance(NUM_NODES, NUM_NODES);
                    Eigen::SparseLU<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int>> solver;
                    Eigen::SparseMatrix<double> sparse;

                    for (double t = 0; t <= tranStopTime; t += tranStepTime)
                    {
                        Math::getConductanceTRAN(schem, conductance, param, t, tranStepTime);
                        Math::getCurrentTRAN(schem, current, conductance, param, t, tranStepTime);

                        // std::cerr << conductance << std::endl;
                        // std::cerr << current << std::endl;

                        sparse = conductance.sparseView();
                        sparse.makeCompressed();
                        solver.analyzePattern(sparse);
                        solver.factorize(sparse);
                        voltage = solver.solve(current);

                        // std::cerr << voltage << std::endl;

                        for_each(schem->nodes.begin(), schem->nodes.end(), [&](const auto node_pair) {
                            if (node_pair.second->getId() != -1)
                            {
                                node_pair.second->voltage = voltage[node_pair.second->getId()];
                            }
                        });

                        if (format == SPACE)
                        {
                            spicePrint(param, t, tranStepTime);
                        }
                        else if (format == CSV)
                        {
                            csvPrint(param, t, tranStepTime);
                        }
                    }
                }
                else
                {
                    const int NUM_NODES = schem->nodes.size() - 1;
                    Eigen::VectorXd voltageOld(NUM_NODES);
                    Eigen::VectorXd voltageNew(NUM_NODES);
                    Eigen::VectorXd current(NUM_NODES);
                    Eigen::MatrixXd conductance(NUM_NODES, NUM_NODES);
                    Eigen::SparseLU<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int>> solver;
                    Eigen::SparseMatrix<double> sparse;

                    Circuit::Math::init_vector(voltageOld);
                    for (double t = 0; t <= tranStopTime; t += tranStepTime)
                    {
                        double error = 1e10;
                        int num_iter = 0;
                        while (error > errorThreshold)
                        {
                            Math::getConductanceTRAN(schem, conductance, param, t, tranStepTime);


                            Math::getCurrentTRAN(schem, current, conductance, param, t, tranStepTime);

                            // std::cerr << conductance << std::endl;
                            // std::cerr << current << std::endl;

                            sparse = conductance.sparseView();
                            sparse.makeCompressed();
                            solver.analyzePattern(sparse);
                            solver.factorize(sparse);

                            voltageNew = solver.solve(current);
                            // std::cerr << voltageNew << std::endl;

                            for_each(schem->nodes.begin(), schem->nodes.end(), [&](const auto node_pair) {
                                if (node_pair.second->getId() != -1)
                                {
                                    node_pair.second->voltage = voltageNew[node_pair.second->getId()];
                                }
                            });

                            error = Circuit::Math::MSE(voltageOld, voltageNew);
                            std::cerr << t << " " << ++num_iter << ": " << error << std::endl;
                            voltageOld = voltageNew;
                        }
                        if (format == SPACE)
                        {
                            spicePrint(param, t, tranStepTime);
                        }
                        else if (format == CSV)
                        {
                            csvPrint(param, t, tranStepTime);
                        }
                    }
                }
            }
            if (format == SPACE)
            {
                dst << spiceStream.str();
            }
            else if (format == CSV)
            {
                dst << csvStream.str();
            }
        }
    }
};
#endif
