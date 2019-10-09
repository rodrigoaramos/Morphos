#include "HistG.h"
#include <vector>

using namespace std;

namespace geometry
{

	// ******************************************************************
	// Score Implementation
	// ******************************************************************

	Score::Score()
	{
	}

	Score::Score(int idx) : index(idx)
	{
	}

	Score::~Score()
	{
	}

	// ******************************************************************
	// HistG Implementation
	// ******************************************************************

	HistG::HistG() : _rowLine((vector<Score>&)vector<Score>()), _colLine((vector<Score>&)vector<Score>())
	{
		//
	}

	HistG::HistG(Mat& m) : _rowLine((vector<Score>&)vector<Score>()), _colLine((vector<Score>&)vector<Score>())
	{
		analise(m);
	}

	HistG::~HistG()
	{
		//
	}

	void HistG::analise(Mat& m)
	{
		_maxRowBlk = _maxColBlk = _maxRowWht = _maxColWht = NULL;
		_minRowBlk = _minColBlk = _minRowWht = _minColWht = NULL;
		Score* scoreMaxblk = NULL;
		Score* scoreMaxwht = NULL;
		Score* scoreMinblk = NULL;
		Score* scoreMinwht = NULL;
		Score* score;
		double black = 0, white = 0;
		size_t rr = _rows = m.rows;
		size_t cc = _cols = m.cols;
		/** Column first */
		for (size_t r = 0; r < rr; r++)
		{
			for (size_t c = 0; c < cc; c++)
			{
				Scalar intensity = m.at<uchar>(r, c);  // y, x
				if (intensity.val[0] == 0)
					black++;
				if (intensity.val[0] == 255)
					white++;
			}
			score = new Score(r);
			score->pixelsBlack = black;
			score->pixelsWhite = white;
			addRow(*score);
			scoreMaxblk = (scoreMaxblk != NULL ? (scoreMaxblk->pixelsBlack > black ? scoreMaxblk : score) : score);
			scoreMaxwht = (scoreMaxwht != NULL ? (scoreMaxwht->pixelsWhite > white ? scoreMaxwht : score) : score);
			/** */
			scoreMinblk = (scoreMinblk != NULL ? (black == 0 || scoreMinblk->pixelsBlack < black ? scoreMinblk : score) : score);
			scoreMinwht = (scoreMinwht != NULL ? (white == 0 || scoreMinwht->pixelsWhite < white ? scoreMinwht : score) : score);
			black = white = 0;
		}
		_maxColBlk = scoreMaxblk;
		_maxColWht = scoreMaxwht;
		/** */
		_minColBlk = scoreMinblk;
		_minColWht = scoreMinwht;
		black = white = 0;
		scoreMaxblk = scoreMaxwht = NULL;
		scoreMinblk = scoreMinwht = NULL;
		/** Row last */
		for (size_t c = 0; c < cc; c++)
		{
			for (size_t r = 0; r < rr; r++)
			{
				Scalar intensity = m.at<uchar>(r, c);
				if (intensity.val[0] == 0)
					black++;
				if (intensity.val[0] == 255)
					white++;
			}
			score = new Score(c);
			score->pixelsBlack = black;
			score->pixelsWhite = white;
			addColumn(*score);
			scoreMaxblk = (scoreMaxblk != NULL ? (scoreMaxblk->pixelsBlack > black ? scoreMaxblk : score) : score);
			scoreMaxwht = (scoreMaxwht != NULL ? (scoreMaxwht->pixelsWhite > white ? scoreMaxwht : score) : score);
			/** */
			scoreMinblk = (scoreMinblk != NULL ? (black == 0 || scoreMinblk->pixelsBlack < black ? scoreMinblk : score) : score);
			scoreMinwht = (scoreMinwht != NULL ? (white == 0 || scoreMinwht->pixelsWhite < white ? scoreMinwht : score) : score);
			black = white = 0;
		}
		_maxRowBlk = scoreMaxblk;
		_maxRowWht = scoreMaxwht;
		/** */
		_minRowBlk = scoreMinblk;
		_minRowWht = scoreMinwht;
	}

	void HistG::addRow(Score& sc) const
	{
		vector<Score>* vt = (vector<Score>*)&(_rowLine);
		vt->push_back(sc);
	}

	void HistG::addColumn(Score& sc) const
	{
		vector<Score>* vt = (vector<Score>*)&(_colLine);
		vt->push_back(sc);
	}

	vector<Score> HistG::getRowLine() const
	{
		return _rowLine;
	}

	vector<Score> HistG::getColLine() const
	{
		return _colLine;
	}

	int HistG::getRows() const
	{
		return _rows;
	}

	int HistG::getColumns() const
	{
		return _cols;
	}

	Score HistG::getMaxRowBlack() const
	{
		*_maxRowBlk = (_maxRowBlk != NULL ? *_maxRowBlk : Score());
		return *_maxRowBlk;
	}

	Score HistG::getMaxColBlack() const
	{
		*_maxColBlk = (_maxColBlk != NULL ? *_maxColBlk : Score());
		return *_maxColBlk;
	}

	Score HistG::getMaxRowWhite() const
	{
		*_maxRowWht = (_maxRowWht != NULL ? *_maxRowWht : Score());
		return *_maxRowWht;
	}

	Score HistG::getMaxColWhite() const
	{
		*_maxColWht = (_maxColWht != NULL ? *_maxColWht : Score());
		return *_maxColWht;
	}

	Score HistG::getMinRowBlack() const
	{
		*_minRowBlk = (_minRowBlk != NULL ? *_minRowBlk : Score());
		return *_minRowBlk;
	}

	Score HistG::getMinColBlack() const
	{
		*_minColBlk = (_minColBlk != NULL ? *_minColBlk : Score());
		return *_minColBlk;
	}

	Score HistG::getMinRowWhite() const
	{
		*_minRowWht = (_minRowWht != NULL ? *_minRowWht : Score());
		return *_minRowWht;
	}

	Score HistG::getMinColWhite() const
	{
		*_minColWht = (_minColWht != NULL ? *_minColWht : Score());
		return *_minColWht;
	}

}