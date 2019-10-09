#pragma once

#include <iostream>
#include <vector>

#include "Neuron.h"
#include "Matrix.h"

using namespace std;

namespace neuralnet
{
	class Layer
	{
	public:
		Layer(int size);
		Layer(int size, int activationType);
		void setVal(int i, double v);

		Matrix *matrixifyVals();
		Matrix *matrixifyActivatedVals();
		Matrix *matrixifyDerivedVals();

		vector<double> getActivatedVals();

		vector<Neuron *> getNeurons() { return this->neurons; };
		void setNeuron(vector<Neuron *> neurons) { this->neurons = neurons; }
	private:
		int size;
		vector<Neuron *> neurons;
	};
}