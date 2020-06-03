#ifndef GUARD_CIRCUIT_SIMULATOR_HPP
#define GUARD_CIRCUIT_SIMULATOR_HPP
#include <sstream>
#include <iostream>
#include <Eigen/Dense>

#include <unsupported/Eigen/NonLinearOptimization>
#include <unsupported/Eigen/NumericalDiff>

template<typename _Scalar, int NX = Eigen::Dynamic, int NY = Eigen::Dynamic>
struct Functor
{

// Information that tells the caller the numeric type (eg. double) and size (input / output dim)
typedef _Scalar Scalar;
enum { // Required by numerical differentiation module
	InputsAtCompileTime = NX,
	ValuesAtCompileTime = NY
};

// Tell the caller the matrix sizes associated with the input, output, and jacobian
typedef Eigen::Matrix<Scalar,InputsAtCompileTime,1> InputType;
typedef Eigen::Matrix<Scalar,ValuesAtCompileTime,1> ValueType;
typedef Eigen::Matrix<Scalar,ValuesAtCompileTime,InputsAtCompileTime> JacobianType;

// Local copy of the number of inputs
int m_inputs, m_values;

// Two constructors:
Functor() : m_inputs(InputsAtCompileTime), m_values(ValuesAtCompileTime) {}
Functor(int inputs, int values) : m_inputs(inputs), m_values(values) {}

// Get methods for users to determine function input and output dimensions
int inputs() const { return m_inputs; }
int values() const { return m_values; }

};
struct ConductanceFunc : Functor<double>
{
	// Simple constructor
	double time = 0;
	Circuit::Schematic* schem;
	Circuit::ParamTable* param;
	double timestep;
	ConductanceFunc(Circuit::Schematic * schem, Circuit::ParamTable *param, double time, double timestep, int NUM_NODES): Functor<double>(NUM_NODES, NUM_NODES) {
		this->schem = schem;
		this->param = param;
		this->timestep = timestep;
		this->time = time;
	}
	// Implementation of the objective function
	// void multiply(Eigen::MatrixXd& conductance ,Eigen::VectorXd& voltage, Eigen::VectorXd& result ){
	// 	int NUM_NODES = voltage.size();
	// 	for(int v = 0; v < NUM_NODES; v++){
	// 		for(int m = 0; m <  conductance.cols(); m++){
				
	// 		}
	// 	}
	// }
	int operator()(const Eigen::VectorXd &voltage, Eigen::VectorXd &fvec) const {
		const int NUM_NODES = schem->nodes.size() - 1;
		std::map<int, Circuit::Node*> saveVolts;
		Eigen::MatrixXd conductance(NUM_NODES, NUM_NODES);
		Circuit::Math::getConductanceTRAN(schem, conductance, param, time, timestep);

		Eigen::VectorXd current(NUM_NODES);
		Circuit::Math::getCurrentTRAN(schem, current, conductance, param, time, timestep);

		// minimize Ax-b
		fvec = conductance*voltage - current;
		// fvec = voltage - conductance.inverse()*current;	
		//std::cerr<<a<<",("<<current.transpose()<<")"<<std::endl;
		return 0;
	}
};


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
					std::cerr<<NUM_NODES<<std::endl;
					int percent = 0;
					Eigen::VectorXd voltageOld(NUM_NODES);
					Circuit::Math::init_vector(voltageOld);
					Eigen::VectorXd diff(NUM_NODES);
					for (double t = 0; t <= tranStopTime; t += tranStepTime)
					{
						if( (t/tranStopTime)/(0.01 *percent)>=1){
							std::cerr<<percent<<"%"<<std::endl;
							percent++;
						}
						ConductanceFunc functor(schem, param, t,tranStepTime, NUM_NODES);
						Eigen::NumericalDiff<ConductanceFunc> numDiff(functor);
						Eigen::MatrixXd jaq(NUM_NODES,NUM_NODES);
						numDiff.df(voltageOld, jaq);
						for(int i = 0; i< 100;i++){
							bool breakflag = false;
							Eigen::VectorXd funcResult(NUM_NODES);
							functor(voltageOld, funcResult);
							Eigen::MatrixXd inverse = jaq.transpose().inverse();
							auto gradFuncResult = (inverse)*funcResult;
							for(int x =0; x< NUM_NODES;x++){
								if(std::isnan(gradFuncResult(x))){
									breakflag = true;
									break;
								}
							}
							if(!breakflag){
								voltageOld = voltageOld - gradFuncResult;
								diff = gradFuncResult;

							}
							else{
								voltageOld = voltageOld - diff;
							}
							
							for(int x =0; x< NUM_NODES;x++){
								if(diff(x)==0){
									diff(x)=1e-12;
								}
							}
						}
						
						for_each(schem->nodes.begin(), schem->nodes.end(), [&](const auto node_pair) {
							if (node_pair.second->getId() != -1)
							{
								node_pair.second->voltage = voltageOld[node_pair.second->getId()];
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
