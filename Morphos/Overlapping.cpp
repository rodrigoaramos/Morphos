#include "Overlapping.h"

namespace match
{

	xGrid::xGrid()
	{

	}

	xGrid::xGrid(int r, int c)
	{
		this->rows = r;
		this->cols = c;
		this->size = (r * c);
		this->slots = new double[this->size];
		for (int i = 0; i < this->size; i++)
		{
			*(this->slots + i) = 0.00;
		}
	}

	xGrid::~xGrid()
	{
	}

	double xGrid::getSlot(int i, int j)
	{
		if (this->size <= 0 || i < 0 || j < 0) return 0L;
		int jump = ((i * this->rows) + j);
		if (jump >= this->size) return 0L;
		return *(this->slots + jump);
	}

	void xGrid::setSlot(int i, int j, double d)
	{
		if (this->size <= 0 || i < 0 || j < 0) return;
		int jump = ((i * this->rows) + j);
		if (jump >= this->size) return;
		*(this->slots + jump) = d;
	}

	void xGrid::printSlots()
	{
		if (this->size <= 0)
		{
			printf("\nSlots is empty.\n\n");
			return;
		}
		printf("\t");
		int len = this->size;
		double * ptds = this->slots;
		for (int i = 0; i < this->rows; i++)
		{
			printf("%d\t", i);
		}
		printf("\n------------------------------------------");
		for (int i = 0; i < len; i++)
		{
			if ((i % this->rows) == 0)
			{
				printf("\n%d\t", (int)floor(i / this->rows));
			}
			double distance = *(ptds + i);
			printf("%.2f\t", distance);
		}
		printf("\n------------------------------------------\n\n");
	}

	/** ================================================================== */

	xPoint::xPoint()
	{
	}

	xPoint::xPoint(double x, double y)
	{
		this->px = x;
		this->py = y;
	}

	xPoint::~xPoint()
	{
	}

	/** ================================================================== */

	xRegion::xRegion()
	{
		this->ptCenter = new xPoint;
	}

	xRegion::xRegion(const Rect & rc) : xRegion()
	{
		this->rect = (Rect*)&rc;
		this->ptCenter->px = ((rc.tl().getX() + floor(rc.width / 2)) / PIXELMETER);
		this->ptCenter->py = ((rc.tl().getY() + floor(rc.height / 2)) / PIXELMETER);
		this->_tl = new xPoint((rc.tl().getX() / PIXELMETER), (rc.tl().getY() / PIXELMETER));
		this->_br = new xPoint((rc.br().getX() / PIXELMETER), (rc.br().getY() / PIXELMETER));
		this->width = (rc.width / PIXELMETER);
		this->height = (rc.height / PIXELMETER);
		this->hypotenuse = (sqrt(pow(rc.width, 2) + pow(rc.height, 2)) / PIXELMETER);
	}

	xRegion::~xRegion()
	{
	}

	xPoint & xRegion::tl()
	{
		return *this->_tl;
	}

	xPoint & xRegion::br()
	{
		return *(this->_br);
	}

	/** ================================================================== */

	xVertex::xVertex()
	{
		this->pointFrom = new xPoint();
		this->pointTo = new xPoint();
		this->fromRegion = new xRegion();
		this->toRegion = new xRegion();
	}

	xVertex::xVertex(const xPoint & pa, const xPoint & pb)
	{
		this->pointFrom = (xPoint*)&(pa);
		this->pointTo = (xPoint*)&(pb);
		this->fromRegion = new xRegion();
		this->toRegion = new xRegion();
	}

	xVertex::~xVertex()
	{
	}

	/** ================================================================== */

	Overlapping::Overlapping()
	{
	}

	Overlapping::~Overlapping()
	{
	}

	Orientation Overlapping::verifyOrientation(const xPoint & pa, const xPoint & pb)
	{
		if (pb.px == pa.px && pb.py == pa.py)
			return Orientation::Center;
		if (pb.px == pa.px && pb.py < pa.py)
			return Orientation::Nort;
		if (pb.px == pa.px && pb.py > pa.py)
			return Orientation::South;
		if (pb.px < pa.px && pb.py == pa.py)
			return Orientation::West;
		if (pb.px > pa.px && pb.py == pa.py)
			return Orientation::East;
		if (pb.px < pa.px && pb.py < pa.py)
			return Orientation::Northwest;
		if (pb.px < pa.px && pb.py > pa.py)
			return Orientation::SouthWest;
		if (pb.px > pa.px && pb.py < pa.py)
			return Orientation::NorthEast;
		if (pb.px > pa.px && pb.py > pa.py)
			return Orientation::SouthEast;
		return Orientation::Unknown;
	}

	string Overlapping::parseOrientation(const Orientation orient)
	{
		if (orient == Orientation::Center)
			return "Center";
		if (orient == Orientation::Nort)
			return "Nort";
		if (orient == Orientation::South)
			return "South";
		if (orient == Orientation::West)
			return "West";
		if (orient == Orientation::East)
			return "East";
		if (orient == Orientation::Northwest)
			return "Northwest";
		if (orient == Orientation::NorthEast)
			return "NorthEast";
		if (orient == Orientation::SouthEast)
			return "SouthEast";
		return "Unknown";
	}

	void Overlapping::couplingGraphs(vector<xRegion*> vregsa, vector<xRegion*> vregsb)
	{
		if (vregsa.size() == 0 || vregsb.size() == 0) return;
		int ixa = 0, ixb = 0;
		Affinity * aff = new Affinity;

		//vector<Graph*>::iterator ita = (vghsa.size() < vghsb.size() ? vghsa.begin() + (ixa) : vghsb.begin() + (ixb));
		//vector<Graph*>::iterator itb = (vghsa.size() > vghsb.size() ? vghsa.begin() + (ixa) : vghsb.begin() + (ixb));
		//double scale = calculeScale(*(*ita), *(*itb));
		//(*itb)->setScale(scale);

		findAnchorRegion(vregsa, &ixa, vregsb, &ixb, *aff);

		//printf("Similar: %s \n", (aff->isSimilar ? "Y" : "N"));
	}

	void Overlapping::createvetorialSpace(vector<geometry::Rect*> & vrcts, vector<xRegion*> & vregs)
	{
		cv::Mat image(cv::Size(626, 311), CV_8U, cv::Scalar::all(255));

		int cc = 0;
		double x1 = 0, y1 = 0, x2 = 0, y2 = 0, opposite = 0, adjacent = 0;
		double xx1 = 0, yy1 = 0, xx2 = 0, yy2 = 0;
		xRegion * vrct;
		for (vector<Rect*>::iterator it = vrcts.begin(); it != vrcts.end(); ++it)
		{
			Rect * rc = *it;
			vrct = new xRegion(*rc);
			vrct->order = cc;
			vregs.push_back(vrct);
			cc++;
		}
		xVertex * vertex;
		for (vector<xRegion*>::iterator it = vregs.begin(); it != vregs.end(); ++it)
		{
			xRegion * xrega = *it;
			x1 = xrega->ptCenter->px;
			y1 = xrega->ptCenter->py;
			xx1 = (x1 - xrega->tl().px);
			yy1 = (y1 - xrega->tl().py);
			for (vector<xRegion*>::iterator itt = vregs.begin(); itt != vregs.end(); ++itt)
			{
				xRegion * xregb = *itt;
				if (xrega->order == xregb->order) continue;
				x2 = xregb->ptCenter->px;
				y2 = xregb->ptCenter->py;
				xx2 = (x2 - xrega->br().px);
				yy2 = (y2 - xrega->br().py);
				vertex = new xVertex;
				vertex->pointFrom->px = 0;
				vertex->pointFrom->py = 0;
				vertex->pointTo->px = (x2 - x1);
				vertex->pointTo->py = (y2 - y1);
				opposite = (xx2 - xx1);
				adjacent = (yy2 - yy1);
				vertex->fromRegion = xrega;
				vertex->toRegion = xregb;
				vertex->opposite = opposite;
				vertex->adjacent = adjacent;
				vertex->hypotenuse = sqrt(pow(abs(adjacent), 2) + pow(abs(opposite), 2));
				vertex->orientation = verifyOrientation(*(new xPoint(x1, y1)), *(new xPoint(x2, y2)));
				xrega->vertexes.push_back(vertex);
				/** */
				//cv::line(image, cv::Point(vertex->pointFrom->px, vertex->pointFrom->py), cv::Point(vertex->pointTo->px, vertex->pointTo->py), cv::Scalar(0), 1, 8, 0);
			}
		}
		//imwrite("C:\\Imagens\\Teste\\Test1.png", image);
	}

	void Overlapping::resetAffinity(Affinity & aff)
	{
		aff.isScaled = false;
		aff.scale = 1L;
	}

	void Overlapping::findAnchorRegion(vector<xRegion*> vregsa, int * ixa, vector<xRegion*> vregsb, int * ixb, Affinity & aff)
	{
		if (*ixa < 0 || *ixa >= vregsa.size()) return;
		if (*ixb < 0 || *ixb >= vregsb.size()) return;
		if (vregsa.size() == 0 || vregsb.size() == 0) return;
		bool isequ = false;
		xRegion * ela = (*(vregsa.begin() + (*ixa)));
		xRegion * elb = (*(vregsb.begin() + (*ixb)));
		while (true)
		{
			isequ = compareBounds(*ela, *elb, aff);
			if (!isequ)
			{
				resetAffinity(aff);
				continue;
			}
			compareVetorialSpace(*ela, *elb, aff);
			//if (aff.isEquals)
			//{
			//	//deepVertexChecker(*ela, *elb, aff);
			//	// check affinity
			//	break;
			//}
			(*ixb)++;
			if ((*ixb) >= vregsb.size())
			{
				/** */
				(*ixa)++;
				if ((*ixa) >= vregsa.size())
				{
					if (!aff.isScaled)
					{
						aff.isScaled = true;
						aff.scale = calculeScale(*ela, *elb);
						*ixa = 0; *ixb = 0;
						ela = (*(vregsa.begin()));
						elb = (*(vregsb.begin()));
						continue;
					}
					// Rotate
					break;
				}
				ela = (*(vregsa.begin() + (*ixa)));
				/** */
				*ixb = 0;
			}
			elb = (*(vregsb.begin() + (*ixb)));
		}
	}

	bool Overlapping::calculeScale(xRegion & vrega, xRegion & vregb)
	{
		bool isdiff = false;
		double delta1 = 0L, delta2 = 0L, delta3 = 0L, delta4 = 0L, scale = 1L;
		delta1 = (vrega.width > 0 && vregb.width > 0 ? (vrega.width / vregb.width) : (vrega.width == vregb.width ? 0L : 1L));
		delta2 = (vrega.height > 0 && vregb.height > 0 ? (vrega.height / vregb.height) : (vrega.height == vregb.height ? 0L : 1L));
		delta3 = (vrega.hypotenuse > 0 && vregb.hypotenuse > 0 ? (vrega.hypotenuse / vregb.hypotenuse) : (vrega.hypotenuse == vregb.hypotenuse ? 0L : 1L));
		if ((delta1 < ERROR_MARGE || delta1 > (1 + (1 - ERROR_MARGE))) ||
			(delta2 < ERROR_MARGE || delta2 > (1 + (1 - ERROR_MARGE))) ||
			(delta3 < ERROR_MARGE || delta3 > (1 + (1 - ERROR_MARGE))))
		{
			scale = (vrega.hypotenuse > 0 && vregb.hypotenuse > 0 ? (vrega.hypotenuse / vregb.hypotenuse) : (vrega.hypotenuse == vregb.hypotenuse ? 0L : 1L));
		}
		return scale;
	}

	bool Overlapping::compareBounds(xRegion & vrega, xRegion & vregb, Affinity & aff)
	{
		double average = (vrega.width > 0 && vregb.width > 0 ? (vrega.width / (vregb.width * aff.scale)) : 0L);
		bool isCheck = (average >= ERROR_MARGE && average <= (1 + (1 - ERROR_MARGE)));
		if (isCheck)
		{
			average = (vrega.height > 0 && vregb.height > 0 ? (vrega.height / (vregb.height * aff.scale)) : 0L);
			isCheck = (average >= ERROR_MARGE && average <= (1 + (1 - ERROR_MARGE)));
			if (isCheck)
			{
				average = (vrega.hypotenuse > 0 && vregb.hypotenuse > 0 ? (vrega.hypotenuse / (vregb.hypotenuse * aff.scale)) : 0L);
				isCheck = (average >= ERROR_MARGE && average <= (1 + (1 - ERROR_MARGE)));
			}
		}
		return isCheck;
	}

	void Overlapping::compareVetorialSpace(xRegion & vrega, xRegion & vregb, Affinity & aff)
	{
		double axisZ = 10000;
		double frxa = 0, frya = 0, toxa = 0, toya = 0, frxb = 0, fryb = 0, toxb = 0, toyb = 0;
		double coefa = 1L, coefb = 0, dtx = 0, dty = 0;
		/** */
		xVertex *verta = NULL, *vertb = NULL, *nvert = NULL;
		vector<xVertex*>* vertsa = &(vrega.vertexes);
		vector<xVertex*>* vertsb = &(vregb.vertexes);
		/** */
		for (vector<xVertex*>::iterator ita = vertsa->begin(); ita != vertsa->end(); ++ita)
		{
			verta = *ita;
			frxa = verta->pointFrom->px;
			frya = verta->pointFrom->py;
			toxa = verta->pointTo->px;
			toya = verta->pointTo->py;
			for (vector<xVertex*>::iterator itb = vertsb->begin(); itb != vertsb->end(); ++itb)
			{
				vertb = *itb;
				frxb = (floor((vertb->pointFrom->px * aff.scale) * 10L) / 10L);
				fryb = (floor((vertb->pointFrom->py * aff.scale) * 10L) / 10L);
				toxb = (floor((vertb->pointTo->px * aff.scale) * 10L) / 10L);
				toyb = (floor((vertb->pointTo->py * aff.scale) * 10L) / 10L);
				dty = ((toyb - axisZ) - toya);
				dtx = ((toxb - axisZ) - toxa);
				coefb = (dty != 0 && dtx != 0 ? (dty / dtx) : 0L);
				//double diff = abs(coefa - coefb);
				//bool isequals = ((coefa == coefb || diff <= ERROR_COEFICIENT) && verta->orientation == vertb->orientation);



				//mindiff = std::min(mindiff, 1L + abs(coefa - coefb));

				string orienta = parseOrientation(verta->orientation);
				string orientB = parseOrientation(vertb->orientation);
				double diff = abs(coefa - coefb);
				bool isequals = ((coefa == coefb || diff <= ERROR_COEFICIENT) && verta->orientation == vertb->orientation);
				printf("Scale: %.4f / Diff: %.4f / isequals: %s / PointA X: %.0f / Y: %.0f | PointB X: %.0f / Y: %.0f / CoeficientA: %.18f / CoeficientB: %.18f / OrientationA: %s / OrientationB: %s / %s \n", aff.scale, diff, (isequals ? "YES" : "NO"), toxa, toya, toxb, toyb, coefa, coefb, orienta.c_str(), orientB.c_str(), (isequals ? "Is Equals" : ""));

			}
			printf("-------------------------------------------------------------\n");

			//if (isequals) cctverts++;
			//bool isequals = (aff.isEquals ? ((coefa == coefb || diff <= ERROR_COEFICIENT) && verta->orientation == vertb->orientation) : false);

		}
		//aff.similarity = (aff.isEquals ? (sumverts > 0 && cctverts > 0 ? (sumverts / cctverts) : 0L) : 0L);
	}

	void Overlapping::printRegions(vector<xRegion*> & vregs)
	{
		for (vector<xRegion*>::iterator it = vregs.begin(); it != vregs.end(); ++it)
		{
			xRegion * region = *it;
			printf("REGION: Order: %d - Center X: %.8F - Center Y: %.8F - Width: %.8F - Height: %.8F - Hypotenuse: %.8F \n", region->order, region->ptCenter->px, region->ptCenter->py, region->width, region->height, region->hypotenuse);
			vector<xVertex*> verts = region->vertexes;
			for (vector<xVertex*>::iterator itt = verts.begin(); itt != verts.end(); ++itt)
			{
				xVertex * vertexs = *itt;
				xRegion toregion = *(vertexs->toRegion);
				printf("----- VERTEX: PointFrom X: %.8F - PointFrom Y: %.8F - PointTo X: %.8F - PointTo Y: %.8F - Order: %d - Width: %.8F - Height: %.8F - Orientation: %d \n", vertexs->pointFrom->px, vertexs->pointFrom->py, vertexs->pointTo->px, vertexs->pointTo->py, toregion.order, toregion.width, toregion.height, (int)vertexs->orientation);
			}
		}
		printf("-------------------------------------------------------------\n");
	}


}