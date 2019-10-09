#pragma once

#include <vector>
#include <algorithm>

using namespace std;

namespace geometry
{
	enum NodeType
	{
		TP_UNDEFINED = 0,
		TP_CONTAINER = 1,
		TP_LINE = 2,
		TP_CHARACTER = 3
	};

	class NodeRect
	{
	public:
		NodeRect();
		~NodeRect();
		NodeType& getType() const;
		void setType(NodeType r);
		double getWidth() const;
		void setWidth(double v);
		double getHeight() const;
		void setHeight(double v);
		double getArea() const;
		void setArea(double area);
		double getHypotenuse() const;
		void setHypotenuse(double v);
		double getCoSine() const;
		void setCoSine(double v);
		double getReason() const;
		void setReason(double v);
		bool hasChilds() const;
		vector<NodeRect*>& GetChilds();
	private:
		NodeType _tpctner = NodeType::TP_UNDEFINED;
		double _width = 0;
		double _height = 0;
		double _area = 0;
		double _hypot = 0;
		double _cosine = 0;
		double _reason = 0;
		vector<NodeRect*> _childs;
	};
}

