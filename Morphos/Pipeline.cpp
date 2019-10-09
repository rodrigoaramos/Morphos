#include "Pipeline.h"

namespace neuralnet
{

	Pipeline::Pipeline()
	{
	}

	Pipeline::~Pipeline()
	{
	}

	bool compareVert(Rect* r1, Rect* r2)
	{
		return (r1->x < r2->x);
	}

	bool compareHoriz(Rect* r1, Rect* r2)
	{
		return (r1->y < r2->y);
	}

	LayerGrid* Pipeline::findLayer(vector<LayerGrid*>* layers, int axis)
	{
		for (vector<LayerGrid*>::iterator it = layers->begin(); it != layers->end(); ++it)
		{
			LayerGrid* lay = *it;
			if (lay->getAxis() == axis)
				return lay;
		}
		return NULL;
	}

	void Pipeline::gridClassifier(vector<Rect*> vrects)
	{
		sort(vrects.begin(), vrects.end(), compareVert);
		int minval_x = INIT_MINIMUM, maxval_x = 0, minval_y = INIT_MINIMUM, maxval_y = 0;
		for (vector<Rect*>::iterator it = vrects.begin(); it != vrects.end(); ++it)
		{
			Rect* rc = *it;
			int x1 = rc->tl().getX();
			int x2 = rc->br().getX();
			int y1 = rc->tl().getY();
			int y2 = rc->br().getY();
			minval_x = (x1 > 0 ? min(x1, minval_x) : minval_x);
			maxval_x = max(x2, maxval_x);
			minval_y = (y1 > 0 ? min(y1, minval_y) : minval_y);
			maxval_y = max(y2, maxval_y);
		}
		/** */
		int marge_v = minval_x;
		int winVerti = (minval_x < INIT_MINIMUM ? (maxval_x - minval_x) : maxval_x);
		int marge_h = minval_y;
		int winHoriz = (minval_y < INIT_MINIMUM ? (maxval_y - minval_y) : maxval_y);
		int stepsVer = ceil(winVerti * LAYER_GAP);
		int stepsHor = ceil(winHoriz * LAYER_GAP);
		/** */
		NodeGrid* node = NULL;
		LayerGrid* lay = NULL;
		vector<LayerGrid*>* vlayers = &(this->getVerticalLayers());
		for (vector<Rect*>::iterator it = vrects.begin(); it != vrects.end(); ++it)
		{
			Rect* rc = *it;
			if ((rc->width - marge_v) == winVerti) continue;
			int posx = rc->tl().getX();
			int axis = ceil((posx - marge_v) / stepsVer);
			lay = findLayer(vlayers, axis);
			if (lay == NULL)
			{
				lay = new LayerGrid(DirectionLine::VERTICAL);
				lay->setAxis(axis);
				vlayers->push_back(lay);
			}
			node = new NodeGrid(0);
			lay->getNodes().push_back(node);
		}
		/** */
		for (vector<LayerGrid*>::iterator it = vlayers->begin(); it != vlayers->end(); ++it)
		{
			LayerGrid* lay = *it;
			vector<NodeGrid*>* nodes = &(lay->getNodes());
			for (vector<NodeGrid*>::iterator itt = nodes->begin(); itt != nodes->end(); ++itt)
			{
				NodeGrid* nd = *itt;
				printf("Axis: %d - Value: %.4f \n", lay->getAxis(), nd->getValue());
			}
		}
	}
}