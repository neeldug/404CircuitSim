#ifndef GUARD_DIODE_HPP
#define GUARD_DIODE_HPP

#include <cmath>

class Circuit::Diode : public Circuit::Component
{
private:
	std::string modelName = "D";
	double *v_across = new double;
	bool *isnan = new bool;

public:
	class ParasiticCapacitance : public Circuit::Capacitor
	{
	public:
		ParasiticCapacitance(Schematic *schem)
		{
			this->schem = schem;
		}
		void setDiodeOwner(Diode *d);
		void setCap(double vGuess, const double &CJ0, const double &VJ)
		{
			this->value = CJ0 / pow(1.0 - vGuess / VJ, 0.5);
		}
		void setNodes(Node *pos, Node *neg)
		{
			this->nodes.push_back(pos);
			this->nodes.push_back(neg);
		}
		virtual ~ParasiticCapacitance() {}
	};

	ParasiticCapacitance *para_cap;
	//REVIEW will probably have to make these doubles and might make this a nested class
	double IS = 1e-14; //also stored in value (Component base class)
	double RS = 1;
	double CJ0 = 4e-12;
	double TT = 0;
	double BV = 100;
	double IBV = 0.1e-10;
	double VJ = 1;
	const double GMIN = 1e-15;
	const double V_T = 25e-3;
	Diode() = default;
	Diode(std::string name, std::string nodeA, std::string nodeB, std::string model, Schematic *schem) : Circuit::Component(name, 0.0, schem)
	{
		para_cap = new ParasiticCapacitance(schem);
		schem->setupConnections2Node(this, nodeA, nodeB);
		para_cap->setNodes(this->getPosNode(), this->getNegNode());
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
		if (*isnan)
		{
			i_prev = *v_across * GMIN + para_cap->getCurrentSource(param, timestep);
		}
		else
		{
			i_prev = IS * (exp(*v_across / V_T) - 1) + para_cap->getCurrentSource(param, timestep);
		}
		return i_prev;
	}

	double getCurrent(ParamTable *param, double time, double timestep) const override
	{
		return i_prev;
	}
	std::string getModelName()
	{
		return modelName;
	}
	~Diode()
	{
		delete v_across;
		delete isnan;
		delete para_cap;
	}
	double getConductance(ParamTable *param, double timestep) const override
	{

		double V_POS = this->getPosNode()->voltage;
		double iNew;
		double vGuess = 10e-3;
		double vNew;
		while (true)
		{
			vNew = V_T * std::log(((V_POS - vGuess * (1 + RS * GMIN)) / (RS * IS)) + 1);
			double error = std::pow(vGuess - vNew, 2);
			if (error < 1e-15 || std::isnan(vNew))
				break;
			vGuess = vNew;
		}
		if (std::isnan(vNew))
		{
			*v_across = ((V_POS / RS) + IS) / ((1 / RS) + GMIN);
			*isnan = true;
			para_cap->setCap(*v_across, CJ0, VJ);
			iNew = *v_across * GMIN;
		}
		else
		{
			*v_across = vNew;
			*isnan = false;
			para_cap->setCap(*v_across, CJ0, VJ);
			iNew = IS * (exp(vNew / V_T) - 1) + *v_across * GMIN;
		}

		return (iNew / *v_across) + para_cap->getConductance(param, timestep);
	}
};
#endif
