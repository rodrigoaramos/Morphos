#pragma once

#define TANH 1
#define RELU 2
#define SIGM 3

#include <iostream>
#include <math.h>

namespace neuralnet
{
	class Neuron
	{
	public:
		Neuron(double val);
		Neuron(double val, int activationType);

		void setVal(double v);
		// Fast sigmoid function: f(x) = x / (1 + |x|)
		void activate();
		// Derive for fast sigmoid function: f'(x) = f(x) * (1 - f(x))
		void derive();

		// Getter
		double getVal() { return this->val; }
		double getActivatedVal() { return this->activatedVal; }
		double getDerivedVal() { return this->derivedVal; }
	private:
		double val;
		double activatedVal;
		double derivedVal;
		int activationType = 3;
	};
}