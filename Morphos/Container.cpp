#include "Container.h"

#include <algorithm>

using namespace std;

namespace geometry
{
	Container::Container() : _mrect((Rect&)Rect())  // , _grrc((Rect&)Rect())
	{
		//
	}

	Container::Container(Rect& r) : _mrect(r) //, _grrc((Rect&)Rect())
	{
		//
	}

	Container::~Container()
	{
		//
	}

	const Container& Container::operator = (const Container& ct)
	{
		_mrect = ct.GetRect();
		//_ignore = ct.isIgnored();
		vector<Container> vt = ct.GetChilds();
		for (size_t i = 0; i < vt.size(); i++)
			_childs.push_back(vt[i]);
		return *this;
	}

	Rect& Container::GetRect() const
	{
		return _mrect;
	}

	void Container::SetRect(Rect& r)
	{
		_mrect = r;
	}

	/*Rect& Container::GetGroupRect() const
	{
		return *_grrc;
	}

	void Container::SetGroupRect(Rect& r)
	{
		_grrc = &r;
	}*/

	/*bool Container::isGroup() const
	{
		return _isgroup;
	}

	void Container::SetGroup(bool b)
	{
		_isgroup = b;
	}*/

	ContainerType& Container::GetType() const
	{
		return (ContainerType&)_tpctner;
	}

	void Container::SetType(ContainerType r)
	{
		_tpctner = r;
	}

	bool Container::hasChilds() const
	{
		return (_childs.size() > 0);
	}

	vector<Container>& Container::GetChilds() const
	{
		return (vector<Container>&)_childs;
	}

	void Container::Add(Container& ct)
	{
		_childs.push_back(ct);
	}

}
