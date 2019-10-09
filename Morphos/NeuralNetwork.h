#pragma once

#include <vector>
#include "Layer.h"
#include "Matrix.h"

using namespace std;

namespace neuralnet
{
	class NeuralNetwork
	{
	public:
		NeuralNetwork(vector<int> topology);
		~NeuralNetwork();
		void setCurrentInput(vector<double> input);
		void printToConsole();
	private:
		int _topologySize;
		vector<int> _topology;
		vector<Layer*> _layers;
		vector<Matrix*> _weightMatrices;
		vector<double> _input;
	};
}