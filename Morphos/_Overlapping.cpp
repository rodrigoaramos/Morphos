#include "Overlap.h"

namespace match
{

	BoxShape::BoxShape()
	{

	}


	BoxShape::~BoxShape()
	{

	}

	Overlap::Overlap()
	{
	}


	Overlap::~Overlap()
	{
		/*int minX = MAXVALUE, minY = MAXVALUE;
		for (vector<geometry::Rect*>::iterator it = vrcts.begin(); it != vrcts.end(); ++it)
		{
			geometry::Rect* rc = *it;
			minX = (minX > 0 ? std::min(rc->x, minX) : minX);
			minY = (minY > 0 ? std::min(rc->y, minY) : minY);
		}
		minX = (minX == MAXVALUE ? 0 : minX);
		minY = (minY == MAXVALUE ? 0 : minY);
		sh->x = (rc->x > minX ? ((rc->x - minX) / PIXELMETER) : 0);
		sh->y = (rc->y > minY ? ((rc->y - minY) / PIXELMETER) : 0);
			printf("Shape: X: %d / Y: %d / Width: %d / Height: %d / MinX: %d / MinY: %d / XX: %d / YY: %d \n", rc->x, rc->y, rc->width, rc->height, minX, minY, (rc->x - minX), (rc->y - minY));
		*/
	}

	void Overlap::calcMainArea(vector<geometry::Rect*> vrcts, geometry::Rect & rect)
	{
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
		rect.x = minX;
		rect.y = minY;
		rect.width = (maxX - minX);
		rect.height = (maxY - minY);
	}

	Orientation Overlap::setOrientation(double coefic)
	{
		if (coefic > 0.0)
			return Orientation::Ascendent;
		if (coefic < 0.0)
			return Orientation::Descendent;
		if (coefic == 0.0)
			return Orientation::Horizontal;
		return Orientation::Vertical;
	}

	/*    Formule: angle cosine law

	      +---------------+
	      |             + |
	      |           +   |
	      |         +     |
	   A  |       +       | B
	      |     +   C     |
	      +---+           |
	      | a |           |
	      +---+-----------+

		  c^2 = a^2 + b^2 + 2.a.b.COS(x)

	*/
	void Overlap::loadContainers(vector<geometry::Rect*> vrcts, vector<BoxShape*>& vshaps)
	{
		//printf("Area1\tHypot1\tArea2\tHypot2\tHypot3\tCoeficient\tOrientation\n");
		int cc = 0;
		double hypot = 0, cosine = 0;
		BoxShape * sh;
		for (vector<geometry::Rect*>::iterator it = vrcts.begin(); it != vrcts.end(); ++it)
		{
			geometry::Rect* rc = *it;
			sh = new BoxShape();
			sh->order = cc;
			sh->x = (rc->x / PIXELMETER);
			sh->y = (rc->y / PIXELMETER);
			sh->width = (rc->width / PIXELMETER);
			sh->height = (rc->height / PIXELMETER);
			hypot = sqrt(pow(rc->width, 2) + pow(rc->height, 2));
			sh->hypotenuse = (hypot / PIXELMETER);

			//printf("Hypo1: %.12f - Hypo2: %.12f \n", hypot, sh->hypotenuse);

			sh->cosine = (hypot / rc->width);
			vshaps.push_back(sh);
			cc++;
		}
		/** */
		double adjac = 0, oppos = 0;
		int x = 0, y = 0, widA = 0, hgtA = 0, widB = 0, hgtB = 0, xcA = 0, ycA = 0, xcB = 0, ycB = 0;
		hypot = 0;
		BoxPath * path;
		for (vector<BoxShape*>::iterator it = vshaps.begin(); it != vshaps.end(); ++it)
		{
			BoxShape * sh = *it;
			for (vector<BoxShape*>::iterator itt = vshaps.begin(); itt != vshaps.end(); ++itt)
			{
				BoxShape * nx = *itt;
				if (sh->order == nx->order) continue;
				/** */
				widA = (sh->width * PIXELMETER);
				hgtA = (sh->height * PIXELMETER);
				xcA = floor(widA / 2);
				ycA = floor(hgtA / 2);
				/** */
				widB = (nx->width * PIXELMETER);
				hgtB = (nx->height * PIXELMETER);
				xcB = floor(widB / 2);
				ycB = floor(hgtB / 2);
				/** */
				path = new BoxPath();
				adjac = (xcB - xcA);
				oppos = (ycB - ycA);
				path->adjacent = (abs(adjac) / PIXELMETER);
				path->opposite = (abs(oppos) / PIXELMETER);
				hypot = sqrt(pow(abs(adjac), 2) + pow(abs(oppos), 2));
				path->hypotenuse = (hypot / PIXELMETER);
				/** */
				/*double angle = (pow(adjac, 2) / abs((pow(oppos, 2) + pow(hypot, 2)) - (2 * oppos * hypot)));
				path->angle = angle;*/
				/** */
				double coefic = (oppos > 0 && adjac > 0 ? (oppos / adjac) : 0L);
				path->coeficient = coefic;
				path->orientation = setOrientation(coefic);
				/** */
				sh->paths.push_back(path);

				//printf("%.4f\n", (nx->hypotenuse * PIXELMETER));

				//double area1 = (widA * hgtA);
				//double hypot1 = (sh->hypotenuse * PIXELMETER);
				//double area2 = (widB * hgtB);
				//double hypot2 = (nx->hypotenuse * PIXELMETER);
				//double hypot3 = (path->hypotenuse * PIXELMETER);
				////printf("HypoV1: %.4f - AreaV1: %.4f - HypoV2: %.4f - AreaV2: %.4f - Orientation: %d \n", hypot1, area1, hypot2, area2, (int)path->orientation);
				//printf("%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%d \n", area1, hypot1, area2, hypot2, hypot3, path->coeficient, (int)path->orientation);

				//double factor1 = roundf((sh->width / nx->width) * 10.0L) / 10.0L;
				//double factor2 = roundf((sh->height / nx->height) * 10.0L) / 10.0L;
				////double factor2 = roundf(((sh->width * PIXELMETER) / (nx->width * PIXELMETER)) * 10.0L) / 10.0L;

				//double factor1 = roundf((sh->hypotenuse / nx->hypotenuse) * 10.0L) / 10.0L;
				/*double factor1 = 0;
				double factor2 = roundf(((sh->hypotenuse * PIXELMETER) / (nx->hypotenuse * PIXELMETER)) * 10.0L) / 10.0L;
				printf("SHA::Width: %.4f - SHA::Height: %.4f - SHB::Width: %.4f - SHB::Height: %.4f - Factor1: %.4f - Factor2: %.4f - Coeficient: %.4f - Orientation: %d \n", (sh->width * PIXELMETER), (sh->height * PIXELMETER), (nx->width * PIXELMETER), (nx->height * PIXELMETER), factor1, factor2, coefic, (int)path->orientation);*/
			}
		}
	}

	bool compare(const BoxShape* b1, const BoxShape* b2)
	{
		return (b1->x < b2->x && b1->y < b2->y);
	}

	void Overlap::calcScaleFactor(vector<BoxShape*>& vshpa, vector<BoxShape*>& vshpb, double * factor)
	{
		Point poffs;
		Point pvcto;
		SegmentR3 vectZ1;
		SegmentR3 vectZ2;
		SegmentR3 vectPs;  // Perspective
		sort(vshpa.begin(), vshpa.end(), compare);
		sort(vshpb.begin(), vshpb.end(), compare);
		/** */
		for (vector<BoxShape*>::iterator it = vshpa.begin(); it != vshpa.end(); ++it)
		{
			BoxShape * sha = *it;
			poffs.setX(sha->x);
			poffs.setY(sha->y);
			vectZ1.setAxisY(*(new Point(poffs.getX() + sha->width, 0)));
			for (vector<BoxShape*>::iterator itt = vshpb.begin(); itt != vshpb.end(); ++itt)
			{
				BoxShape * shb = *itt;
				pvcto.setX((shb->x - poffs.getX()));
				pvcto.setY((shb->y - poffs.getY()));
			}
			//printf("X: %.4f - Y: %.4f \n", (sha->x * PIXELMETER), (sha->y * PIXELMETER));
		}


		/*for (vector<BoxShape*>::iterator it = vshpa.begin(); it != vshpa.end(); ++it)
		{
			BoxShape * sha = *it;
			for (vector<BoxShape*>::iterator itt = vshpb.begin(); itt != vshpb.end(); ++itt)
			{
				BoxShape * shb = *itt;
			}
		}*/
	}

	void Overlap::verifyAffinity(vector<BoxShape*>& vshpa, vector<BoxShape*>& vshpb, geometry::Rect & rect)
	{
		int count = 0;
		double fwidth = 0, fheight = 0, deltawd = 0, deltahg = 0;
		BoxPath * path;
		for (vector<BoxShape*>::iterator it = vshpa.begin(); it != vshpa.end(); ++it)
		{
			BoxShape * sha = *it;
			for (vector<BoxShape*>::iterator itt = vshpb.begin(); itt != vshpb.end(); ++itt)
			{
				BoxShape * shb = *itt;
				fwidth = (sha->width / shb->width);
				fheight = (sha->height / shb->height);
				//printf("sha::Width: %.4f - sha::Height: %.4f - shb::Width: %.4f - shb::Height: %.4f - Width: %.4f - Height: %.4f \n", sha->width, sha->height, shb->width, shb->height, fwidth, fheight);
				deltawd += (1 - fwidth);
				deltahg += (1 - fheight);
				count++;
			}
		}
		deltawd = (deltawd != 0 && count > 0 ? (deltawd / (double)count) : 0L);
		deltahg = (deltahg != 0 && count > 0 ? (deltahg / (double)count) : 0L);
		//printf("DWidth: %.12f - DHeight: %.12f \n", deltawd, deltahg);
		/** */
		/*for (vector<BoxShape*>::iterator it = vshpa.begin(); it != vshpa.end(); ++it)
		{
			BoxShape * sh = *it;
			for (vector<BoxShape*>::iterator itt = vshpb.begin(); itt != vshpb.end(); ++itt)
			{
				BoxShape * nx = *itt;
			}
		}*/
	}

	/*void Overlap::loadContainer(geometry::Rect * rc, Shape& sh)
	{
		sh.width = rc->width;
		sh.height = rc->height;
		sh.hypotenuse = sqrt(pow(sh.width, 2) + pow(sh.height, 2));
		sh.perWidth = (rc->width / sh.hypotenuse);
		sh.perHeight = (rc->height / sh.hypotenuse);
		sh.cosine = (sh.hypotenuse / rc->width);
	}*/

	void Overlap::printShape(BoxShape& sh)
	{
		printf("Shape: X: %.12f / Y: %.12f / Width: %.12f / Height: %.12f / Hypot: %.12f / Cosine: %.12f \n", sh.x, sh.y, sh.width, sh.height, sh.hypotenuse, sh.cosine);
	}

	/*void Overlap::xuxuBeleza(vector<geometry::Rect*> vrcts)
	{
		int length = vrcts.size();
		double sumwidth = 0, sumheight = 0, avghorz = 0, avgvert = 0;
		for (vector<geometry::Rect*>::iterator it = vrcts.begin(); it != vrcts.end(); ++it)
		{
			geometry::Rect* rc = *it;
			sumwidth += (rc->width / PIXELMETER);
			sumheight += (rc->height / PIXELMETER);
		}
		avghorz = (sumwidth > 0 && length > 0 ? (sumwidth / length) : 0L);
		avgvert = (sumheight > 0 && length > 0 ? (sumheight / length) : 0L);
		double factor = (avghorz / avgvert);
		printf("Avg Horizontal: %.4f / Avg Vertical: %.4f / Factor: %.4f \n", avghorz, avgvert, factor);
	}*/

	//void Overlap::calculeExtremes(vector<geometry::Rect*> vrcts, RRect & rrc)
	//{
	//	int minX = MAXVALUE, maxX = 0, minY = MAXVALUE, maxY = 0;
	//	for (vector<geometry::Rect*>::iterator it = vrcts.begin(); it != vrcts.end(); ++it)
	//	{
	//		geometry::Rect* rc = *it;
	//		minX = (minX > 0 ? std::min(rc->x, minX) : minX);
	//		maxX = std::max(rc->x, maxX);
	//		minY = (minY > 0 ? std::min(rc->y, minY) : minY);
	//		maxY = std::max(rc->y, maxY);
	//		//printf("X: %d - Y: %d - Width: %d - Height: %d \n", rc->x, rc->y, rc->width, rc->height);
	//	}
	//	minX = (minX == MAXVALUE ? 0 : minX);
	//	minY = (minY == MAXVALUE ? 0 : minY);
	//	rrc.width = ((maxX - minX) / PIXELMETER);
	//	rrc.height = ((maxY - minY) / PIXELMETER);
	//}

	//void Overlap::createTarget(vector<geometry::Rect*> vrcts, geometry::Rect * rect)
	//{
	//	//printf("MinX: %d - MaxX: %d - MinY: %d - MaxY: %d \n", mininumX, maximumX, mininumY, maximumY);

	//	vector<string> list;
	//	string line;
	//	char buffer[255];
	//	snprintf(buffer, 255, "%d,%d,%d,%d\n", minwd, maxwd, minht, maxht);
	//	line.append(buffer);
	//	list.push_back(line);
	//	saveFile(list);

	//	// Identificar razão de proporção
	//	// Identificar similaridades
	//	// Enquadrar pelo maior container do modelo encontrado
	//}

	void Overlap::saveFile(vector<string> list)
	{
		ofstream sout("targets.txt");
		if (sout.is_open())
		{
			int jp = 0;
			for (vector<string>::iterator it = list.begin(); it != list.end(); ++it)
			{
				string line = *it;
				sout << line << (jp == 0 ? "" : "\n");
				jp++;
			}
			sout.close();
		}
	}

	void Overlap::loadFile()
	{
		/*string input_file_name;

		cout << "Give name of input file : ";
		cin >> input_file_name;

		ifstream input_stream(input_file_name.c_str());

		string buffer;
		int rows = 0;

		while (!input_stream.eof())
		{
			getline(input_stream, buffer, '\n');
			if (!input_stream.eof())
				rows = rows + 1;
		}
		cout << "Counted " << rows << " rows." << endl;

		input_stream.close();*/

	}

	/*void Overlap::matchBounds(vector<geometry::Rect*> vrcts, vector<Shape>& vshps)
	{

	}*/
}

/*

		double width = 0, height = 0;
		for (vector<geometry::Rect*>::iterator it = vrcts.begin(); it != vrcts.end(); ++it)
		{
			geometry::Rect* rc = *it;
			width = ((double)rc->width / PIXELMETER);
			height = ((double)rc->height / PIXELMETER);
			printf("W: %d - H: %d - Width: %.8f - Height: %.8f \n", rc->width, rc->height, width, height);
		}


		// W: 20 - H: 10 - Width: 0.5292 - Height: 0.2646
		// W: 20 - H: 10 - Width: 0.0053 - Height: 0.0026

*/


/*

FATOR_WIDTH = (WIDTH / PIXELMETER);

FATOR_HEIGHT = (HEIGHT / PIXELMETER);

REL_WIDTH = (FATOR_WIDTH_RC1 / FATOR_WIDTH_RC2);

REL_HEIGHT = (FATOR_HEIGHT_RC1 / FATOR_HEIGHT_RC2);

WIDTH_1 = (WIDTH_2 * REL_WIDTH)

*/