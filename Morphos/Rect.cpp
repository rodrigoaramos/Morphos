#include "Rect.h"

#include <vector>
#include <algorithm>

using namespace std;

namespace geometry
{
	Rect::Rect()
	{
	}

	Rect::Rect(int _x, int _y, int _width, int _height)
		: x(_x), y(_y), width(_width), height(_height) {}

	Rect::Rect(const Rect& r)
		: x(r.x), y(r.y), width(r.width), height(r.height) {}

	Rect::Rect(Rect&& r) noexcept
		: x(move(r.x)), y(move(r.y)), width(move(r.width)), height(move(r.height)) {}

	Rect::Rect(const Point& pt1, const Point& pt2)
	{
		x = std::min(pt1.getX(), pt2.getX());
		y = std::min(pt1.getY(), pt2.getY());
		width = std::max(pt1.getX(), pt2.getX()) - x;
		height = std::max(pt1.getY(), pt2.getY()) - y;
	}

	Rect::~Rect()
	{
	}

	Rect& Rect::operator = (const Rect& r)
	{
		x = r.x;
		y = r.y;
		width = r.width;
		height = r.height;
		return *this;
	}

	Rect& Rect::operator = (Rect&& r) noexcept
	{
		x = std::move(r.x);
		y = std::move(r.y);
		width = std::move(r.width);
		height = std::move(r.height);
		return *this;
	}

	bool Rect::operator < (const Rect& r)
	{
		return (this->y < r.y);
	}

	Point Rect::tl() const
	{
		return Point(x, y);
	}

	Point Rect::br() const
	{
		return Point(x + width, y + height);
	}

	int Rect::area() const
	{
		const int result = width * height;
		/*CV_DbgAssert(!std::numeric_limits<int>::is_integer
			|| width == 0 || result / width == height);*/
		return result;
	}

	bool Rect::empty() const
	{
		return width <= 0 || height <= 0;
	}

	bool Rect::contains(const Point& pt) const
	{
		return x <= pt.getX() && pt.getX() < x + width && y <= pt.getY() && pt.getY() < y + height;
	}

	bool Rect::contains(const Rect& rc) const
	{
		Point* pt1 = &rc.tl();
		Point* pt2 = &rc.br();
		return (x <= pt1->getX() && pt1->getX() < x + width && y <= pt1->getY() && pt1->getY() < y + height &&
			x <= pt2->getX() && pt2->getX() < x + width && y <= pt2->getY() && pt2->getY() < y + height);
	}

}

