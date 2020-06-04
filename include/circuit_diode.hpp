#ifndef GUARD_DIODE_HPP
#define GUARD_DIODE_HPP
#include <cmath>

class Circuit::Diode : public Circuit::Component
{
private:
	std::string modelName = "D";
	const double GMIN = 1e-10;
	const double V_T = 25e-3;

public:

	class ParasiticCapacitance : public Circuit::Capacitor
	{
	public:
		ParasiticCapacitance(Schematic* schem){
			this->schem = schem;
		}
		void setDiodeOwner(Diode *d);
		void setCap(double vGuess, const double& CJ0, const double& VJ)
		{
			this->value = CJ0 / pow(1.0 - vGuess / VJ, 0.5);
		}
		void setNodes(Node *pos, Node *neg){
			this->nodes.push_back(pos);
			this->nodes.push_back(neg);
		}
		virtual ~ParasiticCapacitance(){}
	};

	ParasiticCapacitance *para_cap;
	//REVIEW will probably have to make these doubles and might make this a nested class
	double IS = 1e-10; //also stored in value (Component base class)
	double RS = 10;
	double CJ0 = 4e-12;
	double TT = 0;
	double BV = 100;
	double IBV = 0.1e-10;
	double VJ = 1;
	Diode() = default;
	Diode(std::string name, std::string nodeA, std::string nodeB, std::string model, Schematic *schem) : Circuit::Component(name, 0.1, schem)
	{
		para_cap = new ParasiticCapacitance(schem);
		schem->setupConnections2Node(this, nodeA, nodeB);
		para_cap->setNodes( this->getPosNode(),this->getNegNode() );
	}
	void assignModel(std::vector<std::string> params)
	{
		//NOTE Remember to update component value!
		//REVIEW Maybe allow params to be variable dependent

		assert(params.size() == 6 && "Incorrect number of diode params");
		IS = stod(params[0]);
		RS = stod(params[1]);
		CJ0 = stod(params[2]);
		TT = stod(params[3]);
		BV = stod(params[4]);
		IBV = stod(params[5]);

		value = IS;
	}

	double getCurrentSource(ParamTable *param, double timestep)
	{
		double vGuess = getVoltage();
		double shockley;
		double exponentialBreakdown;
		shockley = IS * (exp(vGuess / V_T) - 1);
		exponentialBreakdown = -IS * ( (exp(-(BV + vGuess) / V_T) - 1) + BV / V_T );
		i_prev = (shockley + exponentialBreakdown);
		return (shockley + exponentialBreakdown)+para_cap->getCurrentSource( param,timestep );
	}

	double getCurrent(ParamTable *param, double time, double timestep) const override
	{
		return (i_prev + getVoltage() * getConductance(param, timestep));
	}
	std::string getModelName()
	{
		return modelName;
	}
	~Diode()
	{
		delete para_cap;
	}
	double getConductance(ParamTable *param, double timestep) const override
	{
		// double vGuess = getVoltage();
		// para_cap->setCap(vGuess, this->CJ0, this->VJ);
		// double capConductance = para_cap->getConductance(param, timestep);
		double vGuess = 0.7;
		double vNew;
		double V_POS = this->getPosNode()->voltage;
		while (true) {
			std::cerr << "	" << V_POS << " " << vGuess << " " << RS*IS << '\n';
			vNew = V_T * log(((V_POS - vGuess)/(RS*IS)) + 1);
			double error = vGuess - vNew;
			vGuess = vNew;
		}
		double iNew = IS*(exp(vNew/V_T)-1);
		std::cerr << iNew/vNew;
		return (iNew/vNew);
		// std::cerr<<"Cap conductance "<<capConductance<<std::endl;
		// return (GMIN)+capConductance;
	}

};
#endif
