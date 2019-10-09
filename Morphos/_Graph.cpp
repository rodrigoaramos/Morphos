#include "Graph.h"

namespace graph
{
	Graph::Graph() : Graph(5)
	{
	}

	Graph::Graph(int dim)
	{
		this->_order = dim;
		int len = this->_size = pow(this->_order, 2);
		this->_adjs = new double[len];
		for (int i = 0; i < len; i++)
		{
			*(this->_adjs + i) = 0.00;
		}
	}

	Graph::~Graph()
	{
	}

	int Graph::getOrder()
	{
		return this->_order;
	}

	int Graph::getSize()
	{
		return this->_size;
	}

	double * Graph::getAdjacencies()
	{
		return this->_adjs;
	}

	double Graph::getAdjacency(int i, int j)
	{
		if (this->_order <= 0 || i < 0 || j < 0) return 0L;
		int len = this->_size;
		int jump = ((i * this->_order) + j);
		if (jump >= len) return 0L;
		return *(this->_adjs + jump);
	}

	void Graph::setAdjacency(int i, int j, double d)
	{
		if (this->_order <= 0 || i < 0 || j < 0) return;
		int len = this->_size;
		int jump = ((i * this->_order) + j);
		if (jump >= len) return;
		*(this->_adjs + jump) = d;
	}

	double * Graph::transformToMatrix(double * mat, int nord, int i, int j)
	{
		if (mat == NULL || nord <= 0 || i < 0 || j < 0) return NULL;
		int len = pow(nord, 2);
		int offs = (i * j);
		if (offs < 1) return NULL;
		double * mret = new double[offs];
		int r = 0, c = 0, h = 0;
		for (int k = 0; k <= len; k++)
		{
			if (h >= offs) break;
			if (r >= i && c >= j)
			{
				*(mret + h) = *(mat + (k -1));
				h++;
			}
			c++;
			if (k > 0 && (k % nord) == 0)
			{
				r++;
				c = 0;
			}
		}
		return mret;
	}

	void Graph::calcSpectrum()
	{
		double value = 0;
		int len = this->_size;
		int ord = this->_order;
		double * ptds = this->_adjs;
		/** Subtract lambda of matriz values */
		/*for (int i = 0; i < len; i++)
		{
			value = *(ptds + i);
			if (value == 0.0) continue;
			*(ptds + i) = (value * -1);
		}*/
		/** Calc determinant of matrix */
		int m = 0;
		for (int i = 0; i <= ord; i++)
		{
			printf("--------------------------- RIGHT \n");
			for (int j = i +1; j < ord; j++)
			{
				int r = ((i * ord) + j);
				value = *(ptds + i);
				printf("Value: %.4f \n", value);
			}
			printf("--------------------------- LEFT \n");
			for (int j = (ord - i); --j >= 0; )
			{
				int r = ((i * ord) + j);
				value = *(ptds + i);
				printf("Value: %.4f \n", value);
			}
			//for (int j = i + 1; j < ord; j++)
			//{

			//}

			//for (int j = 0; j < ord; j++)
			//{
			//	int r = ((i * ord) + j);
			//	int l = (((ord - i) * ord) + (ord - j));


			//	if (j != i)
			//	{
			//		for (int w = 0; w < nnn; w++)
			//		{
			//			int h = ((j * nord) + w + 1);
			//			int q = ((m * nnn) + w);
			//			*(tpmat + q) = *(matr + h);
			//		}
			//		m++;
			//	}
			//}
			////ans += ptr[i][0] * det(sub, n - 1)*sign;
			//sign = -sign;
		}
	}

	void Graph::printMatrix()
	{
		if (this->_order <= 0)
		{
			printf("\nMatrix is empty.\n\n");
			return;
		}
		printf("\t");
		int len = this->_size;
		double * ptds = this->_adjs;
		for (int i = 0; i < this->_order; i++)
		{
			printf("%d\t", i);
		}
		printf("\n------------------------------------------");
		for (int i = 0; i < len; i++)
		{
			if ((i % this->_order) == 0)
			{
				printf("\n%d\t", (int)floor(i / this->_order));
			}
			double distance = *(ptds + i);
			printf("%.4f\t", distance);
		}
		printf("\n------------------------------------------\n\n");
	}

	void Graph::printMatrix(double * mat, int nord)
	{
		if (mat == NULL || nord <= 0)
		{
			printf("\nMatrix is empty.\n\n");
			return;
		}
		printf("\t");
		int len = pow(nord, 2);
		for (int i = 0; i < nord; i++)
		{
			printf("%d\t", i);
		}
		printf("\n------------------------------------------");
		for (int i = 0; i < len; i++)
		{
			if ((i % nord) == 0)
			{
				printf("\n%d\t", (int)floor(i / nord));
			}
			double distance = *(mat + i);
			printf("%.4f\t", distance);
		}
		printf("\n------------------------------------------\n\n");
	}
}
