#include "LVector.h"

namespace linear
{

	LVector::LVector()
	{
		this->_ptA = new Point(0, 0);
		this->_ptB = new Point(0, 0);
	}

	LVector::~LVector()
	{
	}

	double LVector::getModule()
	{
		/*double scale = this->getFromElement().getScale();
		scale = (scale == 0 ? 1 : scale);
		return (this->_module * scale);*/
		return this->_module;
	}

	Point & LVector::getPointA()
	{
		return *(this->_ptA);
	}

	void LVector::setPointA(const Point & pt)
	{
		this->_ptA = (Point*)&pt;
		this->_factA.x = (this->_ptA->getX() / PIXELMETER);
		this->_factA.y = (this->_ptA->getY() / PIXELMETER);
		calcModule();
	}

	void LVector::calcModule()
	{
		int xa = this->_ptA->getX();
		int ya = this->_ptA->getY();
		int xb = this->_ptB->getX();
		int yb = this->_ptB->getY();
		this->_module = floor(sqrt(pow((xb - xa), 2) + pow((yb - ya), 2)));
	}

	Point & LVector::getPointB()
	{
		return *(this->_ptB);
	}

	void LVector::setPointB(const Point & pt)
	{
		this->_ptB = (Point*)&pt;
		this->_factB.x = (this->_ptB->getX() / PIXELMETER);
		this->_factB.y = (this->_ptB->getY() / PIXELMETER);
		calcModule();
	}

	LElement & LVector::getFromElement()
	{
		LElement tmp;
		return (this->_fromElem != NULL ? *(this->_fromElem) : tmp);
	}

	void LVector::setFromElement(const LElement & el)
	{
		this->_fromElem = (LElement*)&el;
	}

	LElement & LVector::getToElement()
	{
		LElement tmp;
		return (this->_toElem != NULL ? *(this->_toElem) : tmp);
	}

	void LVector::setToElement(const LElement & el)
	{
		this->_toElem = (LElement*)&el;
	}

	LTypeVector & LVector::getType()
	{
		return this->_type;
	}

	void LVector::setType(const LTypeVector & tp)
	{
		this->_type = tp;
	}

	LDirectionVector & LVector::getDirection()
	{
		return this->_direction;
	}

	void LVector::setDirection(const LDirectionVector & dr)
	{
		this->_direction = dr;
	}

	LPoint & LVector::getFactorA()
	{
		return this->_factA;
	}

	LPoint & LVector::getFactorB()
	{
		return this->_factB;
	}

	double LVector::getFModule()
	{
		/*double scale = this->getFromElement().getScale();
		scale = (scale == 0 ? 1 : scale);
		return ((this->_module * scale) / PIXELMETER);*/
		return this->_module;
	}

	/** */

	LElement::LElement()
	{
	}

	LElement::~LElement()
	{
	}

	int LElement::getOrder()
	{
		return this->_order;
	}

	void LElement::setOrder(int v)
	{
		this->_order = v;
	}

	double LElement::getZDimension()
	{
		return this->_zDimens;
	}

	void LElement::setZDimension(double v)
	{
		this->_zDimens = (v != 0.00 ? v : 1);
	}

	Rect & LElement::getRect()
	{
		return *(this->_rect);
	}

	void LElement::setRect(const Rect & rc)
	{
		this->_rect = (Rect*)&rc;
	}

	vector<LVector*> & LElement::getVectors()
	{
		return this->_vectors;
	}

	LVector * LElement::findVector(const LDirectionVector & dr)
	{
		vector<LVector*>* vectors = &(this->getVectors());
		for (vector<LVector*>::iterator it = vectors->begin(); it != vectors->end(); ++it)
		{
			LVector * vca = *it;
			if (vca->getDirection() == dr) return vca;
		}
		return NULL;
	}

	/** */

	LOverlapping::LOverlapping()
	{
	}

	LOverlapping::~LOverlapping()
	{
	}

	LVector * LOverlapping::findVector(vector<LVector*> & vectors, const int order, const LDirectionVector & dr)
	{
		for (vector<LVector*>::iterator it = vectors.begin(); it != vectors.end(); ++it)
		{
			LVector * vca = *it;
			if (vca->getFromElement().getOrder() == order && vca->getDirection() == dr) return vca;
		}
		return NULL;
	}

	bool compareRect(const Rect * b1, const Rect* b2)
	{
		return (b1->tl().getX() < b2->tl().getX() && b1->tl().getY() < b2->tl().getY());
	}

	void LOverlapping::oldcreateSpace(vector<geometry::Rect*> & vrcts, vector<LElement*> & velems, vector<LVector*> & vects)
	{
		int cc = 0;
		LVector * vct;
		sort(vrcts.begin(), vrcts.end(), compareRect);
		int minX = MAXVALUE, maxX = 0, minY = MAXVALUE, maxY = 0;
		for (vector<geometry::Rect*>::iterator it = vrcts.begin(); it != vrcts.end(); ++it)
		{
			geometry::Rect* rc = *it;
			minX = (minX > 0 ? std::min(rc->x, minX) : minX);
			maxX = std::max(rc->x, maxX);
			minY = (minY > 0 ? std::min(rc->y, minY) : minY);
			maxY = std::max(rc->y, maxY);
		}
		minX = (minX == MAXVALUE ? 0 : minX);
		minY = (minY == MAXVALUE ? 0 : minY);
		/** */
		int x = 0, y = 0;
		LElement * elem;
		for (vector<geometry::Rect*>::iterator it = vrcts.begin(); it != vrcts.end(); ++it)
		{
			geometry::Rect* rc = *it;
			elem = new LElement();
			elem->setOrder(cc);
			elem->setRect(*rc);
			/** */
			vct = new LVector();
			vct->setFromElement(*elem);
			vct->setToElement(*elem);
			vct->setPointB(*(new Point(rc->tl().getX() - minX, rc->tl().getY() - minY)));
			vct->setDirection(LDirectionVector::Center);
			x = floor(rc->width / 2);
			y = floor(rc->height / 2);
			vct->setPointA(*(new Point(x, y)));
			elem->getVectors().push_back(vct);
			vects.push_back(vct);
			/** */
			vct = new LVector();
			vct->setFromElement(*elem);
			vct->setToElement(*elem);
			vct->setPointB(*(new Point(rc->tl().getX() - minX, rc->tl().getY() - minY)));
			vct->setDirection(LDirectionVector::LeftUp);
			elem->getVectors().push_back(vct);
			vects.push_back(vct);
			/** */
			vct = new LVector();
			vct->setFromElement(*elem);
			vct->setPointB(*(new Point(rc->br().getX() - minX, rc->tl().getY() - minY)));
			vct->setDirection(LDirectionVector::RightUp);
			elem->getVectors().push_back(vct);
			vects.push_back(vct);
			/** */
			vct = new LVector();
			vct->setFromElement(*elem);
			vct->setToElement(*elem);
			vct->setPointB(*(new Point(rc->br().getX() - minX, rc->br().getY() - minY)));
			vct->setDirection(LDirectionVector::RightDown);
			elem->getVectors().push_back(vct);
			vects.push_back(vct);
			/** */
			vct = new LVector();
			vct->setFromElement(*elem);
			vct->setToElement(*elem);
			vct->setPointB(*(new Point(rc->tl().getX() - minX, rc->br().getY() - minY)));
			vct->setDirection(LDirectionVector::LeftDown);
			elem->getVectors().push_back(vct);
			vects.push_back(vct);
			/** */
			velems.push_back(elem);
			cc++;
		}
		/** */
		int width = 0, height = 0;
		vector<LVector*> vappends;
		for (vector<LVector*>::iterator it = vects.begin(); it != vects.end(); ++it)
		{
			LVector * vca = *it;
			if (vca->getDirection() != LDirectionVector::Center) continue;
			LElement * ela = &(vca->getFromElement());
			Rect* rca = &(ela->getRect());
			for (vector<LVector*>::iterator itt = vects.begin(); itt != vects.end(); ++itt)
			{
				LVector * vcb = *itt;
				if (vcb->getDirection() != LDirectionVector::Center) continue;
				LElement * elb = &(vcb->getFromElement());
				if (ela->getOrder() == elb->getOrder()) continue;
				Rect* rcb = &(elb->getRect());
				vct = new LVector();
				vct->setFromElement(*ela);
				vct->setToElement(*elb);
				vct->setType(LTypeVector::Vertex);
				x = vcb->getPointB().getX();
				y = vcb->getPointB().getY();
				vct->setPointA(*(new Point(x, y)));
				/** */
				width = elb->getRect().width;
				height = elb->getRect().height;
				x = floor(width / 2);
				y = floor(height / 2);
				vct->setPointB(*(new Point(x, y)));
				/** */
				ela->getVectors().push_back(vct);
				vappends.push_back(vct);
			}
		}
		/** */
		for (vector<LVector*>::iterator it = vappends.begin(); it != vappends.end(); ++it)
		{
			vct = *it;
			vects.push_back(vct);
		}
	}

	void LOverlapping::createSpace(vector<geometry::Rect*> & vrcts, vector<LElement*> & velems, vector<LVector*> & vects)
	{

	}

	void LOverlapping::printLinearSpace(vector<LVector*>* vvects)
	{
		for (vector<LVector*>::iterator it = vvects->begin(); it != vvects->end(); ++it)
		{
			LVector * vect = *it;
			LElement * ela = &(vect->getFromElement());
			LElement * elb = &(vect->getToElement());
			printf("Order: %d - PointA-X: %d - PointA-Y: %d - PointB-X: %d - PointB-Y: %d - Direction: %s - Type: %s - OrderTo: %d - ", (int)ela->getOrder(), (int)vect->getPointA().getX(), (int)vect->getPointA().getY(), (int)vect->getPointB().getX(), (int)vect->getPointB().getY(), (int)(vect->getDirection() == LDirectionVector::Center ? "Center" : (vect->getDirection() == LDirectionVector::LeftUp ? "LeftUp" : (vect->getDirection() == LDirectionVector::RightUp ? "RightUp" : (vect->getDirection() == LDirectionVector::RightDown ? "RightDown" : "LeftDown")))), (int)(vect->getType() == LTypeVector::Vertex ? "Vertex" : "Corner"), (int)(vect->getType() == LTypeVector::Vertex ? elb->getOrder() : -9));
			printf("FactorA-X: %.4f - FactorA-Y: %.4f - FactorB-X: %.4f - FactorB-Y: %.4f \n", vect->getFactorA().x, vect->getFactorA().y, vect->getFactorB().x, vect->getFactorB().y);
		}
	}

	void LOverlapping::coupleElements(vector<LElement*> velmsa, vector<LElement*> velmsb)
	{
		/*LVector * vect = NULL;
		vect = this->findVector(0, LDirectionVector::Center);
		printf("Result: %s \n", (vect == NULL ? "IS NULL!" : "FOUND!"));*/

		if (velmsa.size() == 0 || velmsb.size() == 0) return;
		int ixa = 0, ixb = 0;
		Affinity * aff = new Affinity;
		vector<LElement*>::iterator ita = velmsa.begin() + (ixa);
		vector<LElement*>::iterator itb = velmsb.begin() + (ixb);
		LElement * ela = (*ita);
		LElement * elb = (*itb);
		double zDimens = calculeZDimension(*ela, *elb);
		//(*itb)->setScale(scale);
		findPivotElement(velmsa, &ixa, velmsb, &ixb, aff);
		printf("Similar: %s \n", (aff->isSimilar ? "Y" : "N"));
	}

	void LOverlapping::findPivotElement(vector<LElement*> velmsa, int * ixa, vector<LElement*> velmsb, int * ixb, Affinity * aff)
	{
		if (velmsa.size() == 0 || velmsb.size() == 0) return;
		vector<LElement*>::iterator ita = velmsa.begin() + (*ixa);
		vector<LElement*>::iterator itb = velmsb.begin() + (*ixb);
		LElement * ela = (*ita);
		LElement * elb = (*itb);
		while (true)
		{
			bool alike = compareBounds(*ela, *elb);
			if (alike)
			{
				deepVertexChecker(*ela, *elb, aff);
				// check affinity
			}
			//printf("Is similar: %s \n", (alike ? "Y" : "N"));
			(*ixb)++;
			if ((*ixb) >= velmsb.size())
			{
				// Check similarity
				// Rotate
				/** */
				(*ixa)++;
				if ((*ixa) >= velmsa.size())
				{
					aff->isSimilar = !(aff->isDone = true);
					break;
				}
				ita = velmsa.begin() + (*ixa);
				/** */
				(*ixb) = 0;
				itb = velmsb.begin() + (*ixb);
				break;
			}
		}
	}

	void LOverlapping::deepVertexChecker(LElement & elma, LElement & elmb, Affinity * aff)
	{
		/*vector<LVector*>* vectsa = &(elma.getVectors());
		for (vector<LVector*>::iterator itt = vectsa->begin(); itt != vectsa->end(); ++itt)
		{
			LVector * vcba = *itt;
			if (vcba->getDirection() != LDirectionVector::Center) continue;
		}*/
	}

	double LOverlapping::calculeZDimension(LElement & elma, LElement & elmb)
	{
		LVector * vect1 = NULL;
		LVector * vect2 = NULL;
		vect1 = findVector(elma.getVectors(), elma.getOrder(), LDirectionVector::Center);
		if (vect1 != NULL)
		{
			vect2 = findVector(elmb.getVectors(), elmb.getOrder(), LDirectionVector::Center);
			if (vect2 != NULL)
			{
				return (vect1->getFModule() > 0 && vect2->getFModule() > 0 ? (vect1->getFModule() / vect2->getFModule()) : 0L);
			}
		}
		return 1;
	}

	bool LOverlapping::compareBounds(LElement & elma, LElement & elmb)
	{
		double module1 = 0, module2 = 0, average = 0;
		LVector * vect1 = NULL;
		LVector * vect2 = NULL;
		for (int i = 0; i < 5; i++)
		{
			if ((int)LDirectionVector::Center == i) continue;
			vect1 = findVector(elma.getVectors(), elma.getOrder(), (LDirectionVector)i);
			if (vect1 != NULL)
			{
				vect2 = findVector(elmb.getVectors(), elmb.getOrder(), (LDirectionVector)i);
				if (vect2 != NULL)
				{
					//printf("Vect1-module: %.4f - Vect2-module: %.4f\n", vect1->getFModule(), vect2->getFModule());
					module1 += vect1->getFModule();
					module2 += vect2->getFModule();
				}
			}
		}
		average = (module1 > 0 && module2 > 0 ? (module1 / module2) : 0L);
		//printf("Module1: %.12f - Module2: %.12f - Average: %.12f - Error marge min: %.12f - Error marge max: %.12f \n", module1, module2, average, (double)ERROR_MARGE, (double)(1 / ERROR_MARGE));
		return (average > ERROR_MARGE && average < (1 / ERROR_MARGE));
	}
}