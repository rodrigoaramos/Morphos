#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include "Rect.h"
#include "NodeRect.h"
#include "LayerGrid.h"

#define INIT_MINIMUM 999999
#define LAYER_GAP 0.001

using namespace std;
using namespace geometry;

namespace neuralnet
{
	class Pipeline
	{
	public:
		Pipeline();
		~Pipeline();

		void gridClassifier(vector<geometry::Rect*> vrects);

		vector<LayerGrid*> getVerticalLayers() { return this->_verLayers; }
		vector<LayerGrid*> getHorizontalLayers() { return this->_horLayers; }
	private:
		vector<LayerGrid*> _verLayers;
		vector<LayerGrid*> _horLayers;
		LayerGrid* findLayer(vector<LayerGrid*>* layers, int step);
	};
}