#pragma once

#include "Point.h"

namespace geometry
{
	class Rect
	{
	public:
		Rect();
		Rect(int _x, int _y, int _width, int _height);
		Rect(const Rect& r);
		Rect(Rect&& r) noexcept;
		Rect(const Point& pt1, const Point& pt2);
		~Rect();

		Rect& operator = (const Rect& r);
		Rect& operator = (Rect&& r) noexcept;
		bool operator < (const Rect& r);
		Point tl() const;
		Point br() const;

		int area() const;
		bool empty() const;
		bool contains(const Point& pt) const;
		bool contains(const Rect& rc) const;

		int x;
		int y;
		int width;
		int height;
	};
}

