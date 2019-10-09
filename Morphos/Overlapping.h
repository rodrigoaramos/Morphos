#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cmath>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"

#include "Rect.h"

using namespace std;
using namespace geometry;

#define MAXVALUE 999999
//#define PIXELMETER 3779.5275590551
#define PIXELMETER 1

namespace match
{

	enum Orientation
	{
		Unknown,
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

	struct Affinity
	{
		bool isScaled = false;
		double scale = 1L;
		/*bool isDone = false;
		bool isEquals = false;
		double rotate = 0;
		double slope = 0;
		double mirrored = 0;
		double similarity = 0;
		double errors = 0;
		double counter = 0*/;
	};

	class xGrid
	{
	public:
		xGrid();
		xGrid(int r, int c);
		~xGrid();
		int rows;
		int cols;
		int size;
		double * slots;
		double getSlot(int i, int j);
		void setSlot(int i, int j, double d);
		void printSlots();
	};

	class xPoint
	{
	public:
		xPoint();
		xPoint(double x, double y);
		~xPoint();
		double px = 0;
		double py = 0;
	};

	class xRegion;

	class xVertex
	{
	public:
		xVertex();
		xVertex(const xPoint & pa, const xPoint & pb);
		~xVertex();
		xPoint * pointFrom;
		xPoint * pointTo;
		xRegion * fromRegion;
		xRegion * toRegion;
		double opposite = 0;
		double adjacent = 0;
		double hypotenuse = 0;
		Orientation orientation = Orientation::Unknown;
	};

	class xRegion
	{
	public:
		xRegion();
		xRegion(const geometry::Rect & rc);
		~xRegion();
		int order = 0;
		double width = 0;
		double height = 0;
		double hypotenuse = 0;
		bool isMatched = false;
		Affinity * affinity;
		xPoint * ptCenter;
		vector<xVertex*> vertexes;
		geometry::Rect * rect;
		xPoint & tl();
		xPoint & br();
	private:
		xPoint * _tl;
		xPoint * _br;
	};

	class Overlapping
	{
	public:
		Overlapping();
		~Overlapping();
		const double ERROR_MARGE = 0.95;  // 0.993
		const double ERROR_COEFICIENT = 0.095;
		Orientation verifyOrientation(const xPoint & pa, const xPoint & pb);
		string parseOrientation(const Orientation orient);
		void couplingGraphs(vector<xRegion*> vregsa, vector<xRegion*> vregsb);
		void createvetorialSpace(vector<geometry::Rect*> & vrcts, vector<xRegion*> & vregs);
		void resetAffinity(Affinity & aff);
		void findAnchorRegion(vector<xRegion*> vregsa, int * ixa, vector<xRegion*> vregsb, int * ixb, Affinity & aff);
		bool calculeScale(xRegion & vrega, xRegion & vregb);
		bool compareBounds(xRegion & vrega, xRegion & vregb, Affinity & aff);
		void compareVetorialSpace(xRegion & vrega, xRegion & vregb, Affinity & aff);
		void printRegions(vector<xRegion*> & vregs);
	private:
		int counter = 0;
	};
}
