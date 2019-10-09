#pragma once

namespace geometry
{
	class Point
	{
	public:
		Point();
		Point(int vx, int vy);
		Point(const Point& pt);
		~Point();

		Point& operator = (const Point& pt);
		int getX() const;
		void setX(int vx);
		int getY() const;
		void setY(int vy);
	private:
		int _x;
		int _y;
	};

	class SegmentR3
	{
	public:
		SegmentR3();
		SegmentR3(const Point& ptx, const Point& pty, const Point& ptz);
		~SegmentR3();
		Point& getAxisX();
		void setAxisX(const Point& pt);
		Point& getAxisY();
		void setAxisY(const Point& pt);
		Point& getAxisZ();
		void setAxisZ(const Point& pt);
		double getModule();
		void setModule(double md);
	private:
		Point * _ptX;
		Point * _ptY;
		Point * _ptZ;
		double _module;
	};
}
