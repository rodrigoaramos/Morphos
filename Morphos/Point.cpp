#include "Point.h"

namespace geometry
{
	Point::Point()
	{

	}

	Point::Point(int vx, int vy)
	{
		_x = vx; _y = vy;
	}

	Point::Point(const Point& pt)
	{
		_x = pt.getX(); _y = pt.getY();
	}

	Point::~Point()
	{
	}

	Point& Point::operator = (const Point& pt)
	{
		int vx = pt.getX();
		int vy = pt.getY();
		this->setX(vx);
		this->setY(vy);
		return *this;
	}

	int Point::getX() const
	{
		return this->_x;
	}

	void Point::setX(int vx)
	{
		this->_x = (vx < 0 ? 0 : vx);
	}

	int Point::getY() const
	{
		return this->_y;
	}

	void Point::setY(int vy)
	{
		this->_y = (vy < 0 ? 0 : vy);
	}


	/** SegmentR3 Class */

	SegmentR3::SegmentR3()
	{
		this->_ptX = new Point(0, 0);
		this->_ptY = new Point(0, 0);
		this->_ptZ = new Point(0, 0);
	}
	
	SegmentR3::SegmentR3(const Point& ptx, const Point& pty, const Point& ptz)
	{
		this->setAxisX(ptx);
		this->setAxisY(pty);
		this->setAxisZ(ptz);
	}

	SegmentR3::~SegmentR3()
	{
		//
	}

	Point& SegmentR3::getAxisX()
	{
		return *(this->_ptX);
	}

	void SegmentR3::setAxisX(const Point& pt)
	{
		this->_ptX = (Point*)&pt;
	}

	Point& SegmentR3::getAxisY()
	{
		return *(this->_ptY);
	}

	void SegmentR3::setAxisY(const Point& pt)
	{
		this->_ptY = (Point*)&pt;
	}

	Point& SegmentR3::getAxisZ()
	{
		return *(this->_ptZ);
	}

	void SegmentR3::setAxisZ(const Point& pt)
	{
		this->_ptZ = (Point*)&pt;
	}

	double SegmentR3::getModule()
	{
		return this->_module;
	}

	void SegmentR3::setModule(double md)
	{
		this->_module = md;
	}
}
