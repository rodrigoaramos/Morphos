#pragma once

namespace neuralnet
{
	class NodeGrid
	{
	public:
		NodeGrid();
		NodeGrid(double value);
		~NodeGrid();

		double getValue() { return this->_value; }
	private:
		double _value = 0;
	};
}
