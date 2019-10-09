#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

#include "Rect.h"

using namespace std;
using namespace geometry;

#define MAXVALUE 999999
#define PIXELMETER 3779.5275590551

namespace match
{
	enum Orientation
	{
		Undefined,
		Ascendent,   // m > 0
		Descendent,  // m < 0
		Horizontal,  // m = 0
		Vertical     // m = 90 (undefined)
	};

	class BoxShape;

	struct BoxPath
	{
		//double angle;
		double opposite;
		double adjacent;
		double hypotenuse;
		double coeficient;
		Orientation orientation = Orientation::Undefined;
		BoxShape * shape;
	};

	class BoxShape
	{
	public:
		BoxShape();
		~BoxShape();
		int order;
		double x;
		double y;
		double width;
		double height;
		double hypotenuse;
		double cosine;
		vector<BoxPath*> paths;
	private:
	};

	class Overlap
	{
	public:
		Overlap();
		~Overlap();
		Orientation setOrientation(double coefic);
		void loadContainers(vector<geometry::Rect*> vrcts, vector<BoxShape*>& vshaps);
		void calcMainArea(vector<geometry::Rect*> vrcts, geometry::Rect & rect);
		void calcScaleFactor(vector<BoxShape*>& vshpa, vector<BoxShape*>& vshpb, double * factor);
		void verifyAffinity(vector<BoxShape*>& vshpa, vector<BoxShape*>& vshpb, geometry::Rect & rect);
		//void findAnchoredContainers(vector<BoxShape*>& vshpa, vector<BoxShape*>& vshpb, vector<BoxShape*>& vshpb);

		//void loadContainer(geometry::Rect * rc, Shape& sh);
		void printShape(BoxShape& sh);

		//void createTarget(vector<geometry::Rect*> vrcts, geometry::Rect * rect);
		//void matchBounds(vector<geometry::Rect*> vrcts, vector<Shape>& vshps);
		//void xuxuBeleza(vector<geometry::Rect*> vrcts);
	private:
		void saveFile(vector<string> list);
		void loadFile();
	};
}
