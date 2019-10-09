#include "Vertex.h"

namespace match
{
	Vertex::Vertex()
	{
	}

	Vertex::Vertex(const Graph & gp)
	{
		this->setGraph(gp);
	}

	Vertex::~Vertex()
	{
	}

	Graph & Vertex::getGraph()
	{
		return *(this->_graph);
	}

	void Vertex::setGraph(const Graph & gp)
	{
		this->_graph = (Graph*)&gp;
	}

	Orientation & Vertex::getOrientation()
	{
		return this->_orient;
	}

	void Vertex::setOrientation(const Orientation & orien)
	{
		this->_orient = orien;
	}

	double Vertex::getOpposite()
	{
		return this->factor_opp;
	}

	void Vertex::setOpposite(const double v)
	{
		this->factor_opp = v;
	}

	double Vertex::getAdjacent()
	{
		return this->factor_adj;
	}

	void Vertex::setAdjacent(const double v)
	{
		this->factor_adj = v;
	}

	double Vertex::getHypotenuse()
	{
		return this->factor_hypot;
	}

	void Vertex::setHypotenuse(const double v)
	{
		this->factor_hypot = v;
	}

	double Vertex::getFOpposite()
	{
		return (this->factor_opp / PIXELMETER);;
	}

	double Vertex::getFAdjacent()
	{
		return (this->factor_adj / PIXELMETER);;
	}

	double Vertex::getFHypotenuse()
	{
		return (this->factor_hypot / PIXELMETER);;
	}

}
