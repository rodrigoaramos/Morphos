#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cmath>

#include "Rect.h"

using namespace std;
using namespace geometry;

#define PIXELMETER 3779.5275590551
#define MAXVALUE 999999

namespace linear
{
	enum LTypeVector
	{
		Corner,
		Vertex
	};

	enum LDirectionVector
	{
		Center,
		LeftUp,
		RightUp,
		LeftDown,
		RightDown
	};

	struct Affinity
	{
		bool isDone = false;
		bool isSimilar = false;
		double similarity = 0;
		int rotateDegree = 0;
	};

	struct LPoint
	{
		double x;
		double y;
	};

	class LElement;

	class LVector
	{
	public:
		LVector();
		~LVector();
		double getModule();
		geometry::Point & getPointA();
		void setPointA(const geometry::Point & pt);
		geometry::Point & getPointB();
		void setPointB(const geometry::Point & pt);
		LElement & getFromElement();
		void setFromElement(const LElement & el);
		LElement & getToElement();
		void setToElement(const LElement & el);
		LTypeVector & getType();
		void setType(const LTypeVector & tp);
		LDirectionVector & getDirection();
		void setDirection(const LDirectionVector & dr);
		LPoint & getFactorA();
		LPoint & getFactorB();
		double getFModule();
	private:
		double _module = 0;
		geometry::Point * _ptB;
		geometry::Point * _ptA;
		LPoint _factA;
		LPoint _factB;
		LElement * _fromElem = NULL;
		LElement * _toElem = NULL;
		LTypeVector _type = LTypeVector::Corner;
		LDirectionVector _direction = LDirectionVector::Center;
		void calcModule();
	};

	class LElement
	{
	public:
		LElement();
		~LElement();
		int getOrder();
		void setOrder(int v);
		double getZDimension();
		void setZDimension(double v);
		geometry::Rect & getRect();
		void setRect(const geometry::Rect & rc);
		vector<LVector*> & getVectors();
		LVector * findVector(const LDirectionVector & dr);
	private:
		int _order = 0;
		double _zDimens = 1;
		geometry::Rect * _rect = NULL;
		vector<LVector*> _vectors;
	};

	class LOverlapping
	{
	public:
		LOverlapping();
		~LOverlapping();
		const double ERROR_MARGE = 0.97;
		LVector * findVector(vector<LVector*> & vectors, const int order, const LDirectionVector & dr);
		void createSpace(vector<geometry::Rect*> & vrcts, vector<LElement*> & velems, vector<LVector*> & vects);
		void oldcreateSpace(vector<geometry::Rect*> & vrcts, vector<LElement*> & velems, vector<LVector*> & vects);
		void printLinearSpace(vector<LVector*>* vvects);
		void coupleElements(vector<LElement*> velmsa, vector<LElement*> velmsb);
		void findPivotElement(vector<LElement*> velmsa, int * ixa, vector<LElement*> velmsb, int * ixb, Affinity * aff);
		void deepVertexChecker(LElement & elma, LElement & elmb, Affinity * aff);
		double calculeZDimension(LElement & elma, LElement & elmb);
		bool compareBounds(LElement & elma, LElement & elmb);
	private:
		vector<LElement*> _elements;
		vector<LVector*> _vectors;
	};

}