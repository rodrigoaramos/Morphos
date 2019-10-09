#pragma once

#include <vector>

#include "NodeGrid.h"

using namespace std;

namespace neuralnet
{
	enum DirectionLine
	{
		HORIZONTAL = 0,
		VERTICAL = 1
	};

	class LayerGrid
	{
	public:
		LayerGrid();
		LayerGrid(DirectionLine direct);
		~LayerGrid();

		int getStep() { return this->_step; }
		void setStep(int step) { this->_step = step; }
		int getAxis() { return this->_axis; }
		void setAxis(int axis) { this->_axis = axis; }
		DirectionLine getDirection() { return this->_direction; }
		void setDirection(DirectionLine direct) { this->_direction = direct; }
		vector<NodeGrid*>& getNodes() { return this->_vnodes; }
	private:
		int _step = 0;
		int _axis = 0;
		DirectionLine _direction = DirectionLine::HORIZONTAL;
		vector<NodeGrid*> _vnodes;
	};
}
