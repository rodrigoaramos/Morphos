#pragma once

#include <iostream>
#include <list>

using namespace std;

namespace graph
{
	class Graph
	{
	public:
		Graph();
		Graph(int mn);
		~Graph();
		int getOrder();
		int getSize();
		double * getAdjacencies();
		double getAdjacency(int i, int j);
		void setAdjacency(int i, int j, double d);
		void calcSpectrum();
		void printMatrix();
		void printMatrix(double * mat, int nord);
	private:
		int _order;
		int _size;
		double * _adjs;
		double * transformToMatrix(double * mat, int nord, int i, int j);
	};
}
