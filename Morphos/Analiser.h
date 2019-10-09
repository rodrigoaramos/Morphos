#pragma once

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"

#include "Rect.h"
#include "Container.h"
#include "HistG.h"
#include "Pipeline.h"
#include "NodeRect.h"

using namespace cv;
using namespace std;
using namespace neuralnet;

namespace geometry
{
	class ClasseRange
	{
	public:
		ClasseRange();
		~ClasseRange();
		double minimum = 0;
		double maximum = 0;
		double sumvalue = 0;
		double counter = 0;
		double average = 0;
		vector<Container*>* containers;
	};

	struct Range
	{
		double minimum = 0;
		double maximum = 0;
		double counter = 0;
		double average = 0;
	};

	class Analiser
	{

	public:
		Analiser();
		~Analiser();
		//const double HORIZ_CUTTING = 1.3;  // Fator de corte da base de histograma para identificação de linhas horizontais
		//const double VERTC_CUTTING = 1.3;
		//const double INIT_MINIMUM = 999999;
		const double CTNER_FUZZY = 0.3;    // Fator de limite da identificação de container versus data
		const double HIST_HORIZONTAL = 0;
		const double HIST_VERTICAL = 1;
		const double HEIGHT_TEXT = 5; // Altura do texto em linhas
		const double WIDTH_TEXT = 8; // Largura do texto em colunas
		const double TOLERANCE_BASE = 0.5; // 50% tolerância na bse do histograma
		const double MIN_BLACK_LIMIT = 0.05;  // Limite maximo para considerar retangulo vazio no espaçamento entre caracteres: 5% de preto em relação ao total pixels
		const double TOLERANCE_DIGITS = 1.3;    // Margem erro acima do espaçamento entre caracteres
		//const double FATOR_MIN_HEIGHT = 0.333;
		int ProcessImage(const char* filename);
		void ProcessRectangles(vector<geometry::Rect>& rects, vector<Container>* main);
	private:
		void DrawRectanglesInImage(Mat* img, vector<Container*>* lsrng, RNG* rng);
		bool RectangleHierarquizer(vector<Container>* main, Container* nct, int* top, int* left);
		void FillChildsRectangles(Mat* orig, vector<Container>* ctners, vector<Container>* lsctnr);
		void MaskParentRectangles(Mat* orig, Mat* ctnrMat, Container* ctner, vector<Container>* lsctnr);
		void DrawImageResult(Mat* drawing, Container* container, RNG* rng, int* level, ofstream* fstream);
		void CalculeDeviation(vector<Container>* lsctnr, vector<ClasseRange*>& vranges);

		/** Remove da classe de containers os contornos menores que o grupo da maior quantidade de contornos com altura igual */
		void ApplyFilterContainer(vector<ClasseRange*>& vranges);
		void ProcessDataRegions(Mat* orig, vector<ClasseRange*>& vranges, vector<NodeRect*>& vnodes);
		void DetectDataRegions(Mat* orig, Rect* rc, vector<NodeRect*>& vnodes);
		void SplitWordsFromLine(Mat* mline, Rect* rect, Mat* imgsrc, vector<Rect*>& lsdigs, vector<Rect*>& lswords);
		void AjustRectEdge(Mat* digit, Rect* nrect, Rect& ajrect, double* spacing);
		double CalcRelationBetweenPixelsVert(Mat* mline, Rect* rect);

		void DrawHorizImageHistogram(Mat* mregion, Mat1i* hist, double cutpt);
		void DrawVertImageHistogram(Mat* mregion, Mat1i* hist, double cutpt);
		void CalcTopLineForCut(Mat1i* hist, int dim, vector<Range*>* vranges, double* deviation);

		ClasseRange* findRange(vector<ClasseRange*>* ranges, double value);
		Range* findRangeTwo(vector<Range*>* ranges, double value);
		//Container* reallocList(Container* ctner, int* count, Container* next);
		double CalculeHistFactor(Mat1i& hist, int ampl);
		int counter = 0;
		int counter2 = 0;
		ofstream* _fileStream;
		Mat* _morfelem;

		Mat* _source;
	};

}