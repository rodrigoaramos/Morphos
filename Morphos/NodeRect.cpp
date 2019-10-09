#include "NodeRect.h"

namespace geometry
{
	NodeRect::NodeRect()
	{
	}

	NodeRect::~NodeRect()
	{
	}

	NodeType& NodeRect::getType() const
	{
		return (NodeType&)this->_tpctner;
	}

	void NodeRect::setType(NodeType r)
	{
		this->_tpctner = r;
	}

	double NodeRect::getWidth() const
	{
		return this->_width;
	}

	void NodeRect::setWidth(double v)
	{
		this->_width = v;
	}

	double NodeRect::getHeight() const
	{
		return this->_height;
	}

	void NodeRect::setHeight(double v)
	{
		this->_height = v;
	}

	double NodeRect::getArea() const
	{
		return _area;
	}

	void NodeRect::setArea(double area)
	{
		this->_area = area;
	}

	double NodeRect::getHypotenuse() const
	{
		return this->_hypot;
	}

	void NodeRect::setHypotenuse(double v)
	{
		this->_hypot = v;
	}

	double NodeRect::getCoSine() const
	{
		return this->_cosine;
	}

	void NodeRect::setCoSine(double v)
	{
		this->_cosine = v;
	}

	double NodeRect::getReason() const
	{
		return this->_reason;
	}

	void NodeRect::setReason(double v)
	{
		this->_reason = v;
	}

	bool NodeRect::hasChilds() const
	{
		return (this->_childs.size() > 0);
	}

	vector<NodeRect*>& NodeRect::GetChilds()
	{
		return _childs;
	}

}