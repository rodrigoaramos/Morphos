#include "Graph.h"

namespace match
{
	Graph::Graph()
	{
	}

	Graph::Graph(const Rect & rc)
	{
		this->setRect(rc);
	}

	Graph::~Graph()
	{
	}

	int Graph::getOrder()
	{
		return this->_order;
	}

	void Graph::setOrder(int v)
	{
		this->_order = v;
	}

	Rect & Graph::getRect()
	{
		return *(this->_rect);
	}

	void Graph::setRect(const Rect & rc)
	{
		this->_rect = (Rect*)&rc;
		this->_hypot = sqrt(pow(this->_rect->width, 2) + pow(this->_rect->height, 2));
		this->_cosine = (this->_hypot / this->_rect->width);
	}

	vector<Vertex*> & Graph::getVertexes()
	{
		return this->_vertexes;
	}

	double Graph::getX()
	{
		return this->_rect->x;
	}

	double Graph::getY()
	{
		return this->_rect->y;
	}

	double Graph::getWidth()
	{
		return this->_rect->width;
	}

	double Graph::getHeight()
	{
		return this->_rect->height;
	}

	double Graph::getHypotenuse()
	{
		return this->_hypot;
	}

	void Graph::setHypotenuse(double v)
	{
		this->_hypot = v;
	}

	double Graph::getCosine()
	{
		return this->_cosine;
	}

	void Graph::setCosine(double v)
	{
		this->_cosine = v;
	}

	double Graph::getFWidth()
	{
		return (this->_rect->width / PIXELMETER);
	}

	double Graph::getFHeight()
	{
		return (this->_rect->height / PIXELMETER);
	}

	double Graph::getFHypotenuse()
	{
		return (this->_hypot / PIXELMETER);
	}

	double Graph::compareBounds(Graph & gpb, double scale)
	{
		double widthA = this->getRect().width;
		double heightA = this->getRect().height;
		double widthB = (roundf((gpb.getRect().width * scale) * 10.0L) / 10.0L);
		double heightB = (roundf((gpb.getRect().height * scale) * 10.0L) / 10.0L);
		//double widthB = (gpb.getRect().width * scale);
		//double heightB = ceil(gpb.getRect().height * scale);
		double diffW = (widthA > 0 && widthB > 0 ? abs(widthA / widthB) : 0L);
		double diffH = (heightA > 0 && heightB > 0 ? abs(heightA / heightB) : 0L);
		double average = ((diffW + diffH) / 2);
		printf("widthA: %.4f - heightA: %.4f - widthB: %.4f - heightB: %.4f - diffW: %.4f - diffH: %.4f - average: %.4f - average min: %.4f - average max: %.4f \n", widthA, heightA, widthB, heightB, diffW, diffH, average, (double)(average * ERROR_MARGE), (double)(average * (1 + (1 - ERROR_MARGE))));
		return (average > (average * ERROR_MARGE) && average < (average * (1 + (1 - ERROR_MARGE))));
	}
}