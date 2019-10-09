#pragma once

#define PIXELMETER 3779.5275590551

namespace match
{
	enum Orientation
	{
		Center,
		Nort,
		Northwest,
		West,
		SouthWest,
		South,
		SouthEast,
		East,
		NorthEast
	};

	class Graph;

	class Vertex
	{
	public:
		Vertex();
		Vertex(const Graph & gp);
		~Vertex();
		Graph & getGraph();
		void setGraph(const Graph & gp);
		Orientation & getOrientation();
		void setOrientation(const Orientation & orien);
		double getOpposite();
		void setOpposite(const double v);
		double getAdjacent();
		void setAdjacent(const double v);
		double getHypotenuse();
		void setHypotenuse(const double v);

		double getFOpposite();
		double getFAdjacent();
		double getFHypotenuse();
	private:
		Graph * _graph;
		Orientation _orient = Orientation::Center;
		double factor_opp;
		double factor_adj;
		double factor_hypot;
	};
}

