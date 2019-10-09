#pragma once

#include <vector>
//#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

namespace geometry
{
	class Score
	{
	public:
		Score();
		Score(int idx);
		~Score();
		double index;
		double pixelsBlack;
		double pixelsWhite;
	};

	class HistG
	{
	public:
		HistG();
		HistG(Mat& m);
		~HistG();
		void analise(Mat& m);
		vector<Score> getRowLine() const;
		vector<Score> getColLine() const;
		int getRows() const;
		int getColumns() const;
		Score getMaxRowBlack() const;
		Score getMaxColBlack() const;
		Score getMaxRowWhite() const;
		Score getMaxColWhite() const;
		/** */
		Score getMinRowBlack() const;
		Score getMinColBlack() const;
		Score getMinRowWhite() const;
		Score getMinColWhite() const;
	private:
		void addRow(Score& sc) const;
		void addColumn(Score& sc) const;
		int _rows = 0;
		int _cols = 0;
		vector<Score> _rowLine;
		vector<Score> _colLine;
		Score* _maxRowBlk = NULL;
		Score* _maxColBlk = NULL;
		Score* _maxRowWht = NULL;
		Score* _maxColWht = NULL;
		/** */
		Score* _minRowBlk = NULL;
		Score* _minColBlk = NULL;
		Score* _minRowWht = NULL;
		Score* _minColWht = NULL;
	};
}