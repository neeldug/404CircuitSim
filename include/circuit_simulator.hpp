#ifndef GUARD_CIRCUIT_SIMULATOR_HPP
#define GUARD_CIRCUIT_SIMULATOR_HPP
#include <sstream>
#include <Eigen/Dense>

void progressBar(double progress)
{
	int barwidth = 100;
	int pos = barwidth * progress;
	std::cerr << "\rProgress: [";
	for (int i = 0; i < barwidth; i++)
	{
		if (i < pos)
			std::cerr << "=";
		else if (i == pos)
			std::cerr << ">";
		else
			std::cerr << " ";
	}
	std::cerr << "]" << int(progress * 100) + 1 << "%";
}

class Circuit::Simulator
{
private:
	Schematic *schem;
	double tranStopTime;
	double tranSaveStart;
	double tranStepTime;
	std::stringstream spiceStream;
	std::stringstream csvStream;

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
		const int NUM_NODES = schem->nodes.size() - 1;
		Eigen::VectorXd current(NUM_NODES);
		Eigen::MatrixXd conductance(NUM_NODES, NUM_NODES);
		Eigen::SparseLU<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int>> solver;
		Eigen::SparseMatrix<double> sparse;

		for (ParamTable *param : schem->tables)
		{
			if (type == OP)
			{
				Eigen::VectorXd voltage(NUM_NODES);
				Circuit::Math::getConductanceOP(schem, conductance, param);
				Circuit::Math::getCurrentOP(schem, current, conductance, param);

				sparse = conductance.sparseView();
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

				if (!schem->nonLinear && false)
				{

					Eigen::VectorXd voltage(NUM_NODES);

					for (double t = 0; t <= tranStopTime; t += tranStepTime)
					{
						Math::getConductanceTRAN(schem, conductance, param, t, tranStepTime);
						Math::getCurrentTRAN(schem, current, conductance, param, t, tranStepTime);

						sparse = conductance.sparseView();
						sparse.makeCompressed();
						solver.analyzePattern(sparse);
						solver.factorize(sparse);
						voltage = solver.solve(current);
						std::cerr << "Voltage : " << voltage << '\n';
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
					Eigen::VectorXd voltageOld(NUM_NODES);
					Eigen::VectorXd voltage(NUM_NODES);
					for (double t = 0; t <= tranStopTime; t += tranStepTime)
					{
						double vGuess = 1e-12;

						for(int i = 0; i< 100;i++){
							double voltageDiff = 0;
							for(auto x : schem->nonLinearComps){
								x->setConductance(param, tranStepTime, vGuess);
								Math::getConductanceTRAN(schem, conductance, param, t, tranStepTime);
								Math::getCurrentTRAN(schem, current, conductance, param, t, tranStepTime);

								sparse = conductance.sparseView();
								sparse.makeCompressed();
								solver.analyzePattern(sparse);
								solver.factorize(sparse);
								voltage = solver.solve(current);
								double vPos = (x->getPosNode()->getId()!=-1) ? voltage(x->getPosNode()->getId()) : 0;
								double vNeg = (x->getNegNode()->getId()!=-1) ? voltage(x->getNegNode()->getId()) : 0;
								voltageDiff = vPos - vNeg;
								vGuess = vGuess - voltageDiff/(1/(25e-3)*x->IS*(vGuess/x->V_T));
								if(std::isnan(vGuess)){
									vGuess = 1e-12;
								}
							}
							std::cerr<<abs(voltageDiff-vGuess)<<std::endl;
						}
<<<<<<< HEAD

						Math::getConductanceTRAN(schem, conductance, param, t, tranStepTime);
						Math::getCurrentTRAN(schem, current, conductance, param, t, tranStepTime);


						sparse = conductance.sparseView();
						sparse.makeCompressed();
						solver.analyzePattern(sparse);
						solver.factorize(sparse);
						voltage = solver.solve(current);


						for_each(schem->nodes.begin(), schem->nodes.end(), [&](const auto node_pair) {
							if (node_pair.second->getId() != -1)
							{
								node_pair.second->voltage = voltage[node_pair.second->getId()];
							}
						});
=======
						// ConductanceFunc functor(schem, param, t, tranStepTime, NUM_NODES);
						// Eigen::NumericalDiff<ConductanceFunc> numDiff(functor);
						// Eigen::LevenbergMarquardt<Eigen::NumericalDiff<ConductanceFunc>, double> lm(numDiff);
						// // std::cerr<<lm.info();
						// std::cerr<<"\n\n\n\n";
						// lm.parameters.maxfev = 10000;
						// lm.parameters.xtol = 1e-8;
						//
						// int ret = lm.minimize(voltageOld);
						//
						// for_each(schem->nodes.begin(), schem->nodes.end(), [&](const auto node_pair) {
						// 	if (node_pair.second->getId() != -1)
						// 	{
						// 		node_pair.second->voltage = voltageOld[node_pair.second->getId()];
						// 	}
						// });

>>>>>>> b21652bdd7e88a93c674375010b46c1c0f78e5b3

						if (format == SPACE)
						{
							spicePrint(param, t, tranStepTime);
						}
						else if (format == CSV)
						{
							csvPrint(param, t, tranStepTime);
						}
					}
					std::cerr << std::endl;
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
