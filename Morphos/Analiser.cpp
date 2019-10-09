#include "Analiser.h"

namespace geometry
{

	ClasseRange::ClasseRange()
	{
		containers = new vector<Container*>();
	}

	ClasseRange::~ClasseRange()
	{
		//
	}

	/********************************************************************************************/

	Analiser::Analiser()
	{
		_morfelem = new Mat(Size(4, 3), CV_8U, Scalar::all(0));
		_morfelem->at<uint8_t>(0, 1) = 255;
		_morfelem->at<uint8_t>(1, 1) = 255;
		_morfelem->at<uint8_t>(2, 1) = 255;
		_morfelem->at<uint8_t>(0, 2) = 255;
		_morfelem->at<uint8_t>(1, 2) = 255;
		_morfelem->at<uint8_t>(2, 2) = 255;
		bitwise_not(*_morfelem, *_morfelem);
	}


	Analiser::~Analiser()
	{
		//
	}

	bool compare(const Rect& r1, const Rect& r2)
	{
		return (r1.y < r2.y);
	}

	int Analiser::ProcessImage(const char* filename)
	{
		counter = 0;
		//counter = 56;
		
		ofstream fstream;
		_fileStream = &fstream;
		_fileStream->open("C:\\Imagens\\17052019.txt");
		//_fileStream->open("E:\\Trabalho\\Evolution\\Temp_12042019.txt");
		//_fileStream->open("G:\\Trabalho\\Evolution\\Temp_12042019.txt");
		

		Mat src = imread(filename);
		if (src.empty())
		{
			cout << "Could not open or find the image!\n" << endl;
			waitKey();
			return -1;
		}

		_source = &src;

		RNG rng(12345);
		Mat gray, timg;

		cvtColor(src, gray, CV_BGR2GRAY);
		//threshold(gray, timg, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
		threshold(gray, timg, 150, 255, THRESH_BINARY);

		vector<vector<cv::Point>> contours;
		vector<Vec4i> hierarchy;

		findContours(timg, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		vector<vector<cv::Point>> contours_poly(contours.size());
		vector<cv::Rect> boundRect(contours.size());

		vector<Rect> vrects;
		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));
			Point pp1(boundRect[i].tl().x, boundRect[i].tl().y);
			Point pp2(boundRect[i].br().x, boundRect[i].br().y);
			Rect rc(pp1, pp2);
			vrects.push_back(rc);
		}

		Mat drawing;
		src.copyTo(drawing);

		int level = 0;
		/*ofstream fstream;
		fstream.open("E:\\Trabalho\\Evolution\\temp.txt");*/

		vector<Container> ctners;
		vector<Container> lsctnr;
		ProcessRectangles(vrects, (vector<Container>*)&ctners);

		FillChildsRectangles(&timg, (vector<Container>*)&ctners, (vector<Container>*)&lsctnr);

		/*Range* ptr = NULL;
		Range* ranges;
		int ccclas = 0;*/
		vector<ClasseRange*> ranges;
		CalculeDeviation((vector<Container>*)&lsctnr, ranges);
		ApplyFilterContainer(ranges);

		vector<NodeRect*> vnodes;
		ProcessDataRegions(&timg, ranges, vnodes);

		/*vector<geometry::Rect*> vtrcs;
		for (vector<Rect>::iterator it = vrects.begin(); it != vrects.end(); ++it)
		{
			Rect* rc = (Rect*)&(*it);
			vtrcs.push_back(rc);
		}
		Pipeline* p = new Pipeline();
		p->gridClassifier(vtrcs);*/

		//for (vector<ClasseRange*>::iterator it = ranges.begin(); it != ranges.end(); ++it)
		//{
		//	ClasseRange* cls = *it;
		//	//if (cls->minimum > gaprg->maximum) break;
		//	//if (cls->minimum > 0.0541) break;
		//	//if (cls->minimum != 0) continue;
		//	//if (cls->minimum != 0.028735632183908046) continue;
		//	DrawRectanglesInImage(&drawing, cls->containers, &rng);
		//}
		//imwrite("C:\\Imagens\\Imagem_105314_06052019b.png", drawing);
		////imwrite("C:\\Imagens\\Imagem_105314_307042019a.png", drawing);

		//*_fileStream << "Containers: " << counter2 << "\n";

		//imwrite("C:\\Imagens\\Imagem_105314_307042019a.png", drawing);

		//Range* ptr = NULL;
		//ptr = ranges;
		//int cc = ccclas;
		//for (int i = 0; i < cc; i++)
		//{
		//	ptr += i;
		//	//ptr->median = (ptr->counter > 0 ? ptr->sumval / ptr->counter : ptr->median);
		//}

		/*ptr = findRange(ranges, nclasses, ct->_mediaPxs);
		if (ptr == NULL)
			continue;*/
		
		//for (int k = 0; k < lsctnr.size(); k++)
		//{
		//	Container* ctn = &(lsctnr.at(k));
		//	if (ctn->GetType() == ContainerType::CONTAINER)
		//	{
		//		geometry::Rect* rc = &(ctn->GetRect());
		//		Point* pt1 = &(rc->tl());
		//		Point* pt2 = &(rc->br());
		//		cv::Point cvpt1(pt1->getX(), pt1->getY());
		//		cv::Point cvpt2(pt2->getX(), pt2->getY());
		//		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		//		rectangle(drawing, cvpt1, cvpt2, color, 2, 8, 0);
		//	}
		//}
		////imwrite("E:\\Trabalho\\Evolution\\Imagem_105314_12042019a.png", drawing);
		//imwrite("G:\\Trabalho\\Evolution\\Imagem_105314_12042019a.png", drawing);


		/** */
		//for (int k = 0; k < ctners.size(); k++)
		//{
		//	Container* container = &(ctners.at(k));
		//	DrawImageResult(&drawing, container, &rng, &level, &fstream);
		//}
		////imwrite("E:\\Trabalho\\Evolution\\Imagem_105314_12042019a.png", drawing);
		//imwrite("G:\\Trabalho\\Evolution\\Imagem_105314_12042019a.png", drawing);

		//fstream.close();


		_fileStream->close();
		return 0;
	}
	
	void Analiser::DrawRectanglesInImage(Mat* img, vector<Container*>* lsrng, RNG* rng)
	{
		if (lsrng == NULL) return;
		for (vector<Container*>::iterator it = lsrng->begin(); it != lsrng->end(); ++it)
		{
			Container* ctn = *it;
			if (ctn->GetType() == ContainerType::DATA) continue;
			geometry::Rect* rc = &(ctn->GetRect());
			Point* pt1 = &(rc->tl());
			Point* pt2 = &(rc->br());
			cv::Point cvpt1(pt1->getX(), pt1->getY());
			cv::Point cvpt2(pt2->getX(), pt2->getY());
			Scalar color = Scalar(rng->uniform(0, 255), rng->uniform(0, 255), rng->uniform(0, 255));
			rectangle(*img, cvpt1, cvpt2, color, 2, 8, 0);
		}
	}

	void Analiser::ProcessRectangles(vector<geometry::Rect>& rects, vector<Container>* main)
	{
		//ofstream myfile;
		//_fileStream = &myfile;
		//_fileStream->open("E:\\Trabalho\\Evolution\\temp.txt");

		bool added = false;
		sort(rects.begin(), rects.end(), compare);
		Rect* rc;
		Container* ctner;
		int top = 0; int left = 0;
		using Iter = vector<Rect>::const_iterator;
		for (Iter it = rects.begin(); it != rects.end(); ++it)
		{
			rc = (Rect*)&(*it);
			ctner = &(Container(*rc));
			if (main->empty())
			{
				top += rc->tl().getY();
				left += rc->tl().getX();
				main->push_back(*ctner);
				continue;
			}
			added = RectangleHierarquizer(main, ctner, &top, &left);
			if (!added)
			{
				main->push_back(*ctner);
			}
		}

		//_fileStream->close();
	}

	bool Analiser::RectangleHierarquizer(vector<Container>* main, Container* nct, int* top, int* left)
	{
		bool added = false;
		Container* ct;
		vector<Container>* chlds = NULL;
		using TIter = vector<Container>::reverse_iterator;
		for (TIter it = main->rbegin(); it != main->rend(); ++it)
		{
			ct = (Container*)&(*it);
			chlds = &(ct->GetChilds());
			if (!(chlds->empty()))
			{
				added = RectangleHierarquizer(chlds, nct, top, left);
				if (added)
					break;
			}
			Rect* rc = &(ct->GetRect());
			if (rc->contains(nct->GetRect()))
			{
				nct->top += *top + rc->tl().getY();
				nct->left += *left + rc->tl().getX();
				ct->Add(*nct);
				nct = NULL;
				added = true;
				break;
			}

		}
		return added;
	}

	void Analiser::FillChildsRectangles(Mat* orig, vector<Container>* ctners, vector<Container>* lsctnr)
	{
		Mat tmpmap = Mat::zeros(1, 1, CV_8UC1);
		Mat* tempMat = NULL;
		Mat* ctnrMat = &tmpmap;
		for (size_t k = 0; k < ctners->size(); k++)
		{
			Container* ctn = &(ctners->at(k));
			Rect* rc = &(ctn->GetRect());

			tempMat = new Mat(*orig, cv::Rect(rc->x, rc->y, rc->width, rc->height));
			(*tempMat).copyTo(*ctnrMat);

			MaskParentRectangles(orig, ctnrMat, ctn, lsctnr);
		}
	}

	void Analiser::MaskParentRectangles(Mat* orig, Mat* ctnrMat, Container* ctner, vector<Container>* lsctnr)
	{
		Mat presult = Mat::zeros(1, 1, CV_8UC1);
		Mat* parentMat = &presult;
		Mat* temporMat = NULL;
		vector<Container>* chlds;
		chlds = &(ctner->GetChilds());
		for (size_t k = 0; k < chlds->size(); k++)
		{
			Container* ct = &(chlds->at(k));
			Rect* rc = &(ct->GetRect());
			cv::Point cpt1(abs(rc->tl().getX() - ct->left), abs(rc->tl().getY() - ct->top));
			cv::Point cpt2(abs(rc->br().getX() - ct->left), abs(rc->br().getY() - ct->top));
			rectangle(*ctnrMat, cpt1, cpt2, Scalar(255, 255, 255), -1);  // Clearing
			temporMat = new Mat(*orig, cv::Rect(rc->x, rc->y, rc->width, rc->height));
			(*temporMat).copyTo(*parentMat);

			MaskParentRectangles(orig, parentMat, ct, lsctnr);
		}
		Mat1i histvert;
		Mat1b gray;
		double ccount = 0, medpixels = 0, total = 0;
		int steps = 0, ccwhite = 0, ccblack = 0;
		int cols = ctnrMat->cols;
		int rows = ctnrMat->rows;
		total = (double)(cols * rows);
		while (true)
		{
			ccount = medpixels = 0;
			for (int x = 0; x < rows; x++) {
				cv::Mat row = ctnrMat->row(x);
				cv::reduce(row, histvert, 0, CV_REDUCE_SUM);  // 0: Horiz - 1: Vert
				ccblack = (cols - countNonZero(histvert));
				ccount += ccblack;
			}
			medpixels = (total > 0 ? (ccount / total) : 0);
			ctner->_mediaPxs = (ctner->_mediaPxs == 0 ? medpixels : ctner->_mediaPxs);
			if (steps > 3 || medpixels == 0 || medpixels > 0.1)
			{
				if (medpixels == 0)
				{
					ctner->SetType(ContainerType::CONTAINER);
				}
				break;
			}
			bitwise_not(*ctnrMat, gray);
			erode(gray, *ctnrMat, *_morfelem);
			bitwise_not(*ctnrMat, *ctnrMat);
			steps++;
		}
		lsctnr->push_back(*ctner);
	}

	void Analiser::CalculeDeviation(vector<Container>* lsctnr, vector<ClasseRange*>& vranges)
	{
		double minIndex = 0, maxIndex = 0, rangeMin = 0, rangeMax = 0;
		double ampldat = 0, amplcls = 0, average = 0, variance = 0, deviation = 0;
		size_t nclasses = 0, cc = 0;
		minIndex = INIT_MINIMUM;
		cc = lsctnr->size();
		for (vector<Container>::iterator it = lsctnr->begin(); it != lsctnr->end(); ++it)
		{
			Container* ct = &(*it);
			minIndex = min(ct->_mediaPxs, minIndex);
			maxIndex = max(ct->_mediaPxs, maxIndex);
		}
		/** */
		nclasses = (1 + (3.3 * log(cc)));
		ampldat = (maxIndex - minIndex);
		amplcls = (ampldat / ((double)nclasses));
		/** */
		ClasseRange* ptr = NULL;
		rangeMin = minIndex;
		for (size_t i = 0; i < nclasses; i++)
		{
			rangeMax = (i == 0 ? (minIndex + amplcls) : (i == (nclasses -1) ? (minIndex + 0.00000001 + ((i + 1) * amplcls)) : (minIndex + ((i + 1) * amplcls))));
			ptr = new ClasseRange();
			ptr->minimum = rangeMin;
			ptr->maximum = rangeMax;
			ptr->sumvalue = 0;
			ptr->counter = 0;
			ptr->average = 0;
			vranges.push_back(ptr);
			rangeMin = rangeMax;
		}
		/** */
		double sumvalue = 0, counter = 0;
		for (vector<Container>::iterator it = lsctnr->begin(); it != lsctnr->end(); ++it)
		{
			Container* ct = &(*it);
			ptr = findRange(&vranges, ct->_mediaPxs);
			if (ptr == NULL)
				continue;
			if (ct->_mediaPxs > 0)
			{
				ptr->sumvalue += ct->_mediaPxs;
				sumvalue += ct->_mediaPxs;
				ptr->counter++;
				counter++;
			}
			ptr->containers->push_back(ct);
		}
		/** */
		average = (counter > 0 ? (sumvalue / counter) : 0L);
		for (int i = 0; i < nclasses; i++)
		{
			ptr = vranges.at(i);
			ptr->average = (ptr->counter > 0 ? (ptr->sumvalue / ptr->counter) : ptr->sumvalue);
			if (ptr->average == 0) continue;
			variance += pow(abs(ptr->average - average), 2);
		}
		variance = (counter > 0 ? (variance / (counter > 1 ? counter -1 : counter)) : 0L);
		deviation = (variance > 0 ? sqrt(variance) : 0L);
	}

	void Analiser::ApplyFilterContainer(vector<ClasseRange*>& vetrng)
	{
		vector<Container*>* lsrng = NULL;
		ClasseRange* cls = NULL;
		cls = (vetrng.size() > 0 ? vetrng.at(0) : NULL);
		if (cls == NULL)
		{
			return;
		}
		vector<Container*> lsctners;
		lsrng = cls->containers;
		for (vector<Container*>::iterator it = lsrng->begin(); it != lsrng->end(); ++it)
		{
			Container* ctn = *it;
			lsctners.push_back(ctn);
		}
		size_t nclasses = 0, count = 0;
		/** */
		vector<ClasseRange*> nranges;
		double ampldat = 0, amplcls = 0, average = 0, variance = 0, deviation = 0;
		double minIndex = INIT_MINIMUM, maxIndex = 0, rangeMin = 0, rangeMax = 0, sumvalue = 0;
		for (vector<Container*>::iterator it = lsctners.begin(); it != lsctners.end(); ++it)
		{
			Container* ct = (*it);
			double area = (ct->GetRect().width * ct->GetRect().height);
			minIndex = min(area, minIndex);
			maxIndex = max(area, maxIndex);
			sumvalue += area;
		}
		/** */
		count = lsctners.size();
		nclasses = ((1 + 3.3) * log(count));
		ampldat = (maxIndex - minIndex);
		amplcls = (ampldat / ((double)nclasses));
		average = (count > 0 ? (sumvalue / count) : 0L);
		/** */
		ClasseRange* ptr = NULL;
		rangeMin = minIndex;
		for (size_t i = 0; i < nclasses; i++)
		{
			rangeMax = (i == 0 ? (minIndex + amplcls) : (i == (nclasses - 1) ? (minIndex + 0.00000001 + ((i + 1) * amplcls)) : (minIndex + ((i + 1) * amplcls))));
			ptr = new ClasseRange();
			ptr->minimum = rangeMin;
			ptr->maximum = rangeMax;
			ptr->sumvalue = 0;
			ptr->counter = 0;
			ptr->average = 0;
			nranges.push_back(ptr);
			rangeMin = rangeMax;
		}
		/** */
		for (vector<Container*>::iterator it = lsctners.begin(); it != lsctners.end(); ++it)
		{
			Container* ct = (*it);
			double area = (ct->GetRect().width * ct->GetRect().height);
			if (area > 0)
			{
				ptr = findRange(&nranges, area);
				if (ptr == NULL)
					continue;
				ptr->sumvalue += area;
				ptr->counter++;
			}
		}
		/** */
		maxIndex = 0;
		ClasseRange* ptrng = NULL;
		for (vector<ClasseRange*>::iterator it = nranges.begin(); it != nranges.end(); ++it)
		{
			ptr = (*it);
			ptr->average = (ptr->counter > 0 ? (ptr->sumvalue / ptr->counter) : ptr->sumvalue);
			if (ptr->average == 0) continue;
			variance += pow(abs(ptr->average - average), 2);
			if (ptr->counter > maxIndex)
			{
				maxIndex = ptr->counter;
				ptrng = ptr;
			}
		}
		variance = (count > 0 ? (variance / (count > 1 ? count -1 : count)) : 0L);
		deviation = (variance > 0 ? sqrt(variance) : 0L);
		if (ptrng != NULL)
		{
			double maximum = ceil(ptrng->average * CTNER_FUZZY);
			for (vector<ClasseRange*>::iterator it = vetrng.begin(); it != vetrng.end(); ++it)
			{
				ptr = (*it);
				for (vector<Container*>::iterator itc = ptr->containers->begin(); itc != ptr->containers->end(); ++itc)
				{
					Container* ct = (*itc);
					double area = (ct->GetRect().width * ct->GetRect().height);
					if (area <= maximum)
					{
						ct->SetType(ContainerType::DATA);
					}
				}
			}
		}
	}

	void Analiser::ProcessDataRegions(Mat* orig, vector<ClasseRange*>& vranges, vector<NodeRect*>& vnodes)
	{
		vector<Container*>* lsrng;
		double ccount = 0, medpixels = 0, total = 0;
		for (vector<ClasseRange*>::iterator it = vranges.begin(); it != vranges.end(); ++it)
		{
			ClasseRange* cls = *it;
			lsrng = cls->containers;
			for (vector<Container*>::iterator it = lsrng->begin(); it != lsrng->end(); ++it)
			{
				Container* ctn = *it;
				if (ctn->GetType() == ContainerType::DATA) continue;


				//vnodes.push_back();

				Rect* rc = &(ctn->GetRect());
				DetectDataRegions(orig, rc, vnodes);
			}
		}
	}

	void Analiser::DetectDataRegions(Mat* orig, Rect* rc, vector<NodeRect*>& vnodes)
	{
		Mat1i hhorz;
		int ccblacks = 0;
		double cutpt = 0;
		Mat* mtempor = NULL;
		mtempor = new Mat(*orig, cv::Rect(rc->x, rc->y, rc->width, rc->height));
		int rows = mtempor->rows;
		int cols = mtempor->cols;
		Mat1i& hstHoriz = *(new Mat1i(rows, 1));
		for (int x = 0; x < rows; x++) {
			Mat row = mtempor->row(x);
			reduce(row, hhorz, HIST_HORIZONTAL, CV_REDUCE_SUM);
			ccblacks = (cols - countNonZero(hhorz));
			hstHoriz.at<uint8_t>(x, 0) = ccblacks;
		}
		Range* ptr = NULL;
		vector<Range*> vrnghor;
		double deviation = 0;
		CalcTopLineForCut(&hstHoriz, mtempor->cols, &vrnghor, &deviation);
		for (vector<Range*>::iterator it = vrnghor.begin(); it != vrnghor.end(); ++it)
		{
			Range* rng = *it;
			if (ptr == NULL || rng->counter > ptr->counter)
			{
				ptr = rng;
			}
		}
		cutpt = (ptr != NULL ? ceil(ptr->minimum) : mtempor->cols);
		int const BLACK = 0;
		int const WHITE = 1;
		double cutting = (cols - cutpt);
		double minblack = INIT_MINIMUM, minwhite = INIT_MINIMUM;
		double pxblacks = 0, medblack = 0, medwhite = 0;
		int operation = BLACK, lastoper = BLACK, cclines = 0;
		/** Calcula média de altura de brancos e pretos */
		for (int i = 0; i < rows; i++)
		{
			pxblacks = hstHoriz.at<uint8_t>(i, 0);
			pxblacks = (pxblacks <= cutting ? 0 : 1);
			pxblacks = (pxblacks > 0 ? pxblacks : 0);
			operation = (pxblacks == 0 ? WHITE : BLACK);
			bool changed = (lastoper != operation);
			if (changed && cclines > 0)
			{
				if (lastoper == BLACK)
				{
					minblack = min((double)cclines, minblack);
				}
				else
				{
					minwhite = min((double)cclines, minwhite);
				}
				cclines = 0;
				lastoper = operation;
				continue;
			}
			lastoper = operation;
			cclines++;
		}
		medblack = (minblack > WIDTH_TEXT ? ceil(minblack) : WIDTH_TEXT);
		medwhite = (minwhite > HEIGHT_TEXT ? ceil(minwhite) : HEIGHT_TEXT);
		DrawHorizImageHistogram(mtempor, &hstHoriz, cutting);
		Mat* cpmat = NULL;
		cpmat = new Mat(*_source, cv::Rect(rc->x, rc->y, rc->width, rc->height));
		Point* pp1 = NULL;
		pp1 = new Point(-1, -1);
		Point* pp2 = NULL;
		Rect* rect = NULL;
		vector<Rect*> regions;
		bool isWhite = false, isBlack = false, iscascad = false;
		double sumpixels = 0, average = 0;
		int maxval = 0, value = 0, axisY = -1;
		int ccwhite = 0, ccblack = 0;
		if (cutpt > 0)
		{
			for (int i = 0; i < rows; i++)
			{
				value = hstHoriz.at<uint8_t>(i, 0);
				pxblacks = (value <= (cols - cutpt) ? 0 : value);
				pxblacks = (pxblacks > 0 ? pxblacks : 0);
				isWhite = (pxblacks == 0);
				if (!isWhite)
				{
					if (!iscascad)
					{
						bool isFirst = (regions.size() == 0);
						if (isFirst || ccwhite >= medwhite)
						{
							pp1 = new Point(0, i);
							ccblack = 0;
						}
						iscascad = !(isWhite = false);
						isBlack = true;
					}
					else
						ccblack++;
				}
				else
				{
					if (isBlack && ccblack >= medblack && pp1 != NULL && pp1->getY() != -1)
					{
						if (pp1->getY() != axisY)
						{
							pp2 = new Point(cols - 1, i);
							rect = new Rect(*pp1, *pp2);
							regions.push_back(rect);
							axisY = pp1->getY();
						}
						else
						{
							regions.erase(regions.end() - 1);
							pp2 = new Point(cols - 1, i);
							rect = new Rect(*pp1, *pp2);
							regions.push_back(rect);
						}
						/** */
						ccwhite = 0;
						ccblack = 0;
						isBlack = false;
					}
					else
						ccwhite++;
					iscascad = false;
				}
			}
		}
		Rect* nwrct = NULL;
		Mat* imgsrc;
		RNG rngv(12345);
		/** Processa linhas individuais */
		int i = 0;
		Mat hstimg;
		Mat* matline = NULL;
		for (vector<Rect*>::iterator it = regions.begin(); it != regions.end(); ++it)
		{
			rect = *it;
			matline = new Mat(*mtempor, cv::Rect(rect->x, rect->y, rect->width, rect->height));

			imgsrc = new Mat(*cpmat, cv::Rect(rect->x, rect->y, rect->width, rect->height));
			imgsrc->copyTo(hstimg);

			nwrct = new Rect(0, 0, 0, 0);
			vector<Rect*> lsdigs;
			vector<Rect*> lswords;
			SplitWordsFromLine(matline, rect, &hstimg, lsdigs, lswords);
			for (vector<Rect*>::iterator itt = lsdigs.begin(); itt != lsdigs.end(); ++itt)
			{
				nwrct = *(itt);
				Scalar color = Scalar(rngv.uniform(0, 255), rngv.uniform(0, 255), rngv.uniform(0, 255));
				rectangle(*imgsrc, cv::Point(nwrct->tl().getX(), nwrct->tl().getY()), cv::Point(nwrct->br().getX() - 1, nwrct->br().getY() - 1), color, 1, 8, 0);
			}
		}
	}

	void Analiser::SplitWordsFromLine(Mat* mline, Rect* rect, Mat* imgsrc, vector<Rect*>& lsdigs, vector<Rect*>& lswords)
	{
		int ccblacks = 0;
		double cutpt = 0;
		double deviation = 0;
		Mat1i hvert;
		int rows = mline->rows;
		int cols = mline->cols;
		Mat1i& hstVerti = *(new Mat1i(cols, 1));
		for (int x = 0; x < cols; x++) {
			Mat col = mline->col(x);
			reduce(col, hvert, HIST_VERTICAL, CV_REDUCE_SUM);
			ccblacks = (rows - countNonZero(hvert));
			hstVerti.at<uint8_t>(x, 0) = ccblacks;
		}
		/** // Calculo do fator identificação de histograma
		double indexval = 0;
		indexval = CalculeHistFactor(hstVerti, cols);
		*/
		Range* ptr = NULL;
		vector<Range*> vrngvert;
		CalcTopLineForCut(&hstVerti, rows, &vrngvert, &deviation);
		for (vector<Range*>::iterator it = vrngvert.begin(); it != vrngvert.end(); ++it)
		{
			Range* rng = *it;
			if (ptr == NULL || rng->counter > ptr->counter)
			{
				ptr = rng;
			}
		}
		cutpt = (ptr != NULL ? ptr->minimum : rows);
		double cutting = (rows - cutpt);
		RNG testrng(12345);
		Rect* nrect = NULL;
		Point* pp1 = NULL;
		pp1 = new Point(0, 0);
		Point* pp2 = NULL;
		vector<int> spacings;
		vector<Rect*> symbols;
		int maxvalue = 0, minvalue = INIT_MINIMUM;
		double pxblacks = 0, sumvalues = 0, limits = 0, limtest = 0;
		bool isWhite = false, isBlack = false, iscascad = false;
		int ccwhite = 0, ccblack = 0, value = 0, countwh = 0;
		for (int i = 0; i < cols; i++)
		{
			value = hstVerti.at<uint8_t>(i, 0);
			pxblacks = (value <= cutting ? 0 : value);
			pxblacks = (pxblacks > 0 ? pxblacks : 0);
			isWhite = (pxblacks == 0);
			if (!isWhite)
			{
				if (!iscascad)
				{
					if (i > 0)
					{
						nrect = new Rect(*pp1, *(new Point(i, rows)));
						symbols.push_back(nrect);
						pp1 = new Point(i, 0);
						spacings.push_back(ccwhite);
						/** */
						maxvalue = std::max(ccwhite, maxvalue);
						minvalue = (ccwhite > 0 ? std::min(ccwhite, minvalue) : minvalue);
						countwh++;
					}
					ccwhite = 0;
					iscascad = !(isWhite = false);
					isBlack = true;
				}
				else
					ccblack++;
			}
			else
			{
				if (isBlack && ccblack >= WIDTH_TEXT)
				{
					if (i > 0)
					{
						nrect = new Rect(*pp1, *(new Point(i, rows)));
						symbols.push_back(nrect);
						pp1 = new Point(i, 0);
					}
					ccblack = 0;
					isBlack = false;
				}
				ccwhite++;
				iscascad = false;
			}
		}

		if (ccwhite > 0 || isBlack)
		{
			if (isBlack || !iscascad)
			{
				if (pp1 != NULL && pp1->getX() < cols)
				{
					nrect = new Rect(*pp1, *(new Point(cols, rows)));
					symbols.push_back(nrect);
					spacings.push_back(ccwhite);
				}
			}
		}
		/** */
		int pp = 0;
		size_t nclass = (1 + (3.3 * log(countwh)));
		cutting = ceil((maxvalue - minvalue) / nclass);
		sumvalues = 0; maxvalue = 0; minvalue = INIT_MINIMUM; countwh = 0;
		for (vector<int>::iterator it = spacings.begin(); it != spacings.end(); ++it)
		{
			pp = *it;
			if (pp > cutting) continue;
			maxvalue = std::max(pp, maxvalue);
			minvalue = (pp > 0 ? std::min(pp, minvalue) : minvalue);
			sumvalues += pp;
			countwh++;
		}
		limits = (sumvalues > 0 ? ceil((sumvalues / countwh) * TOLERANCE_DIGITS) : 4);
		Mat* digit = NULL;
		Rect* ajrect = NULL;
		double average = 0;
		sumvalues = 0; maxvalue = 0; minvalue = INIT_MINIMUM; countwh = 0;
		for (vector<Rect*>::iterator it = symbols.begin(); it != symbols.end(); ++it)  // Recursive
		{
			nrect = *it;
			average = CalcRelationBetweenPixelsVert(mline, nrect);
			if (average > MIN_BLACK_LIMIT)
			{
				lsdigs.push_back(nrect);
				continue;
			}
			Rect* ajrect = new Rect(0, 0, 0, 0);
			digit = new Mat(*mline, cv::Rect(nrect->x, nrect->y, nrect->width, nrect->height));
			AjustRectEdge(digit, nrect, *ajrect, &limits);
			if (ajrect->width > 0)
			{
				lsdigs.push_back(ajrect);
			}
		}
	}

	void Analiser::AjustRectEdge(Mat* digit, Rect* nrect, Rect& ajrect, double* spacing)
	{
		if (digit == NULL || nrect == NULL) return;
		Mat1i hvert;
		size_t nclass = 0;
		int pxwhite = 0, maxIndex = 0, minIndex = 0, lastcol = 0;
		double sumvalue = 0, ampldat = 0, amplcls = 0, average = 0;
		bool isWhite = false;
		int rows = digit->rows;
		int cols = digit->cols;
		for (int x = 0; x < cols; x++)
		{
			Mat col = digit->col(x);
			reduce(col, hvert, HIST_VERTICAL, CV_REDUCE_SUM);
			pxwhite = countNonZero(hvert);
			lastcol = (!isWhite && pxwhite == rows ? x : lastcol);
			isWhite = (isWhite || pxwhite == rows);
			if (pxwhite < rows)
			{
				isWhite = false;
			}
		}
		if (cols > *spacing)
		{
			if (lastcol < (cols / 2))
			{
				ajrect = *(new Rect(nrect->tl().getX(), nrect->tl().getY(), lastcol, nrect->height));
			}
			else
			{
				ajrect = *(new Rect(lastcol, nrect->tl().getY(), nrect->width, nrect->height));
			}
		}
	}

	double Analiser::CalcRelationBetweenPixelsVert(Mat* mline, Rect* rect)
	{
		int ccblacks = 0, ccwhite = 0;
		double total = 0, average = 0;
		Mat1i hvert;
		Mat* msymb = NULL;
		msymb = new Mat(*mline, cv::Rect(rect->x, rect->y, rect->width, rect->height));
		int rows = msymb->rows;
		int cols = msymb->cols;
		for (int x = 0; x < cols; x++) {
			Mat col = msymb->col(x);
			reduce(col, hvert, HIST_VERTICAL, CV_REDUCE_SUM);
			ccwhite = countNonZero(hvert);
			ccblacks += (rows - ccwhite);
		}
		total = (rows * cols);
		average = (total > 0 && ccblacks > 0 ? ((double)ccblacks / total) : 0L);
		return average;
	}

	void Analiser::CalcTopLineForCut(Mat1i* hist, int dim, vector<Range*>* vranges, double* deviation)
	{
		if (hist == NULL || dim <= 0) return;
		const Mat1i& histdata = *hist;
		size_t nclasses = 0;
		double ampldat = 0, amplcls = 0, average = 0, maximum = 0;
		double minIndex = INIT_MINIMUM, maxIndex = 0, rangeMin = 0, rangeMax = 0, sumvalue = 0;
		int pxblack = 0, pxwhite = 0;
		int axis = histdata.rows;
		for (int i = 0; i < axis; i++)
		{
			pxblack = histdata.at<uint8_t>(i, 0);
			pxwhite = (dim - pxblack);
			minIndex = min((double)pxwhite, minIndex);
			maxIndex = max((double)pxwhite, maxIndex);
			sumvalue += pxwhite;
		}
		nclasses = (1 + (3.3 * log(axis)));
		ampldat = (maxIndex - minIndex);
		amplcls = (ampldat / ((double)nclasses));
		average = (axis > 0 ? (sumvalue / axis) : 0L);
		/** */
		Range* prange = NULL;
		rangeMin = minIndex;
		for (size_t i = 0; i < nclasses; i++)
		{
			rangeMax = (i == 0 ? (minIndex + amplcls) : (i == (nclasses - 1) ? (minIndex + 0.00000001 + ((i + 1) * amplcls)) : (minIndex + ((i + 1) * amplcls))));
			prange = new Range();
			prange->minimum = rangeMin;
			prange->maximum = rangeMax;
			vranges->push_back(prange);
			rangeMin = rangeMax;
		}
		/** */
		Range* ptr = NULL;
		for (int i = 0; i < axis; i++)
		{
			pxblack = histdata.at<uint8_t>(i, 0);
			pxwhite = (dim - pxblack);
			ptr = findRangeTwo(vranges, pxwhite);
			if (ptr == NULL)
				continue;
			ptr->counter++;
			ptr->average += pxwhite;
		}
	}

	void Analiser::DrawHorizImageHistogram(Mat* mregion, Mat1i* histimg, double cutpt)
	{
		if (mregion == NULL || histimg == NULL) return;
		const Mat& regdata = *mregion;
		const Mat1i& histdata = *histimg;
		int maxval = 0, value = 0;
		int histw = 0, histh = 0;
		histh = regdata.rows;
		histw = 500;
		for (int i = 0; i < histh; i++) {
			value = histdata.at<uint8_t>(i, 0);
			maxval = std::max(value, maxval);
		}
		if (maxval == 0)
			return;
		Mat histImage(histh, histw, CV_8UC1, Scalar(255, 255, 255));
		double index = 0;
		for (int i = 0; i < histh; i++)
		{
			value = histdata.at<uint8_t>(i, 0);
			if (value <= cutpt) continue;
			index = ceil(((double)value / maxval) * histw);
			line(histImage, cv::Point(0, i +1), cv::Point(index, i +1), Scalar(0, 0, 0), 1, 8, 0);
		}

	}
	
	Range* Analiser::findRangeTwo(vector<Range*>* ranges, double value)
	{
		for (vector<Range*>::iterator it = ranges->begin(); it != ranges->end(); ++it)
		{
			Range* ptr = *it;
			if (value >= ptr->minimum && value < ptr->maximum)
				return ptr;
		}
		return NULL;
	}

	void Analiser::DrawVertImageHistogram(Mat* mregion, Mat1i* histimg, double cutpt)
	{
		if (mregion == NULL || histimg == NULL) return;
		const Mat& regdata = *mregion;
		const Mat1i& histdata = *histimg;
		uint8_t test = 0;
		int maxval = 0, value = 0;
		int histw = 0, histh = 0;
		histh = 500;
		histw = regdata.cols;
		for (int i = 1; i < histw; i++) {
			value = histdata.at<uint8_t>(i, 0);
			maxval = std::max(value, maxval);
		}
		if (maxval == 0)
			return;
		Mat histImage(histh, histw, CV_8UC1, Scalar(0, 0, 0));
		double index = 0;
		for (int i = 0; i < histw; i++)
		{
			value = histdata.at<uint8_t>(i, 0);
			if (value <= cutpt) continue;
			index = ceil(((double)value / maxval) * histh);
			line(histImage, cv::Point(i + 1, histh), cv::Point(i + 1, histh - index), Scalar(255, 255, 255), 1, 8, 0);
		}
	}

	ClasseRange* Analiser::findRange(vector<ClasseRange*>* ranges, double value)
	{
		for (vector<ClasseRange*>::iterator it = ranges->begin(); it != ranges->end(); ++it)
		{
			ClasseRange* ptr = *it;
			if (value >= ptr->minimum && value < ptr->maximum)
				return ptr;
		}
		return NULL;
	}

	void Analiser::DrawImageResult(Mat* drawing, Container* container, RNG* rng, int* level, ofstream* fstream)
	{
		if (container->GetType() == ContainerType::CONTAINER)
		{
			geometry::Rect* rc = &(container->GetRect());
			Point* pt1 = &(rc->tl());
			Point* pt2 = &(rc->br());
			cv::Point cvpt1(pt1->getX(), pt1->getY());
			cv::Point cvpt2(pt2->getX(), pt2->getY());
			Scalar color = Scalar(rng->uniform(0, 255), rng->uniform(0, 255), rng->uniform(0, 255));
			rectangle(*drawing, cvpt1, cvpt2, color, 2, 8, 0);
		}
		/** */
		int nxlevel = (*level) +1;
		vector<Container>* chlds;
		chlds = &(container->GetChilds());
		for (size_t k = 0; k < chlds->size(); k++)
		{
			Container* ct = &(chlds->at(k));
			DrawImageResult(drawing, ct, rng, &nxlevel, fstream);
		}
	}

	double Analiser::CalculeHistFactor(Mat1i& hsdata, int ccampl)
	{
		if (ccampl > 0 && ccampl > hsdata.rows) return 0L;
		Range* ptrrng = NULL;
		Range* ranger = NULL;
		int h = 1, idxcls = 0, nrclass = 0, clsplus = 0;
		int minval = INIT_MINIMUM, maxval = 0, pixels = 0, cols = 0, downto = 0;
		double sumval = 0, sumfact = 0, ampldat = 0, amplcls = 0;
		cols = hsdata.rows;
		for (int i = 0; i < ccampl; i++)
		{
			if (i >= cols) continue;
			pixels = hsdata.at<uint8_t>(i, 0);
			minval = (pixels > 0 ? std::min(pixels, minval) : minval);
			maxval = std::max(pixels, maxval);
		}
		nrclass = ceil(1 + (3.3 * log(ccampl)));
		ampldat = (maxval > 0 && minval > 0 ? (maxval - minval) : 0L);
		amplcls = (ampldat > 0 && nrclass > 0 ? (ampldat / nrclass) : 0L);
		clsplus = (nrclass + 1);
		ranger = new Range[clsplus];
		for (int i = 0; i < clsplus; i++)
		{
			*(ranger + i) = *(new Range());
		}
		for (int i = 0; i < ccampl; i++)
		{
			if (i >= cols) continue;
			pixels = hsdata.at<uint8_t>(i, 0);
			idxcls = floor((pixels / ampldat) * nrclass);
			ptrrng = (ranger + idxcls);
			ptrrng->counter++;
			ptrrng->average += pixels;
		}
		sumfact = 0;
		for (int i = 0; i < clsplus; i++)
		{
			ptrrng = (ranger + i);
			ptrrng->average = (ptrrng->average > 0 && ptrrng->counter > 0 ? (ptrrng->average / ptrrng->counter) : 0L);
			sumfact += (double)(clsplus * ((clsplus - 1) - i)) + 1;
		}
		double indexval = 0;
		for (int i = 0; i < clsplus; i++)
		{
			ptrrng = (ranger + i);
			downto = (double)(clsplus * ((clsplus - 1) - i)) + 1;
			indexval += (ptrrng->average * downto);
		}
		indexval = (indexval > 0 && sumfact > 0 ? (indexval / (sumfact * 255)) : 0L);
		return indexval;
	}
}

// 1053 Lines 19/06