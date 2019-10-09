#pragma once

#include <vector>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"

#include "Rect.h"

using namespace std;

namespace geometry
{
	enum ContainerType
	{
		DATA = 0,
		CONTAINER = 1
	};

	class Container
	{
	public:
		Container();
		Container(Rect& r);
		~Container();
		const Container& operator = (const Container& ct);
		Rect& GetRect() const;
		void SetRect(Rect& r);
		/*Rect& GetGroupRect() const;
		void SetGroupRect(Rect& r);*/
		ContainerType& GetType() const;
		void SetType(ContainerType r);
		bool hasChilds() const;
		vector<Container>& GetChilds() const;
		void Add(Container& ct);
		//cv::Mat1i* _blackPxs = NULL;
		double _mediaPxs = 0;
		int top = 0;
		int left = 0;
	private:
		Rect& _mrect;
		//Rect* _grrc = NULL;
		ContainerType _tpctner = ContainerType::DATA;
		vector<Container> _childs;
	};
}
