#include "NeuralNetwork.h"

namespace neuralnet
{
	NeuralNetwork::NeuralNetwork(vector<int> topology) : _topology(topology)
	{
		this->_topologySize = topology.size();
		for (int i = 0; i < this->_topologySize; i++)
		{
			Layer* l = new Layer(topology.at(i));
			this->_layers.push_back(l);
		}
		for (int i = 0; i < (this->_topologySize -1); i++)
		{
			Matrix* m = new Matrix(topology.at(i), topology.at(i + 1), true);
			this->_weightMatrices.push_back(m);
		}
	}


	NeuralNetwork::~NeuralNetwork()
	{
	}

	void NeuralNetwork::setCurrentInput(vector<double> input)
	{
		this->_input = input;
		for (int i = 0; i < this->_input.size(); i++)
		{
			this->_layers.at(0)->setVal(i, input.at(i));
		}
	}

	void NeuralNetwork::printToConsole()
	{
		for (int i = 0; i < this->_layers.size(); i++)
		{
			cout << "LAYER: " << i << endl;
			if (i == 0)
			{
				Matrix* m = this->_layers.at(i)->matrixifyVals();
				m->printToConsole();
			}
			else
			{
				/*Matrix* m = this->_layers.at(i)->matrixifyActivateVals();
				m->printToConsole();*/
			}
		}
	}
}
