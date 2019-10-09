#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"

#include "Analiser.h"

#include "Neuron.h"
#include "Matrix.h"
#include "NeuralNetwork.h"
#include "Graph.h"

#include "Overlapping.h"

#define N 4

using namespace cv;
using namespace std;
using namespace geometry;
using namespace neuralnet;
using namespace graph;
using namespace match;

int LoadRectangles(string filename, vector<geometry::Rect*>& vect)
{
	Mat src = imread(filename.c_str());
	if (src.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		waitKey();
		return -1;
	}
	geometry::Rect * rc;

	Mat gray, timg;
	cvtColor(src, gray, CV_BGR2GRAY);
	threshold(gray, timg, 150, 255, THRESH_BINARY);
	vector<vector<cv::Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(timg, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	vector<vector<cv::Point>> contours_poly(contours.size());
	vector<cv::Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));

		if (boundRect[i].x == 0 || boundRect[i].y == 0) continue;
		rc = new geometry::Rect(boundRect[i].x, boundRect[i].y, boundRect[i].width, boundRect[i].height);
		vect.push_back(rc);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	/*Neuron* n = new Neuron(1.5);
	cout << "Val: " << n->getVal() << endl;
	cout << "Activate val: " << n->getActivateVal() << endl;
	cout << "Derive val: " << n->getDeriveVal() << endl;*/

	/*Matrix* m = new Matrix(3, 2, true);
	m->printToConsole();

	cout << "-------------------------------------" << endl;

	Matrix* mT = m->transpose();
	mT->printToConsole();*/


	/*vector<int> topology;
	topology.push_back(3);
	topology.push_back(2);
	topology.push_back(3);

	vector<double> input;
	input.push_back(1.0);
	input.push_back(0.0);
	input.push_back(1.0);

	NeuralNetwork* nn = new	NeuralNetwork(topology);
	nn->setCurrentInput(input);

	nn->printToConsole();*/

	int result = 0;
	Overlapping overlap;
	vector<geometry::Rect*> vect1;
	result = LoadRectangles("C:\\Imagens\\Matriz\\Proto1.png", vect1);
	vector<xRegion*> vrega;
	overlap.createvetorialSpace(vect1, vrega);

	//overlap.printRegions(vrega);

	vector<geometry::Rect*> vect2;
	result = LoadRectangles("C:\\Imagens\\Matriz\\Proto5.png", vect2);
	vector<xRegion*> vregb;
	overlap.createvetorialSpace(vect2, vregb);

	//overlap.printRegions(vregb);

	overlap.couplingGraphs(vrega, vregb);

	//int result = 0;
	//Overlapping overlap;
	//geometry::Rect rect;
	//vector<geometry::Rect*> vect1;
	//result = LoadRectangles("C:\\Imagens\\Matriz\\Proto1.png", vect1);
	//vector<match::Graph*> vgrps1;
	//overlap.createCanonicalSpace(vect1, vgrps1);

	////overlap.printGraphs(vgrps1);

	//vector<geometry::Rect*> vect2;
	//result = LoadRectangles("C:\\Imagens\\Matriz\\Proto1.png", vect2);
	//vector<match::Graph*> vgrps2;
	//overlap.createCanonicalSpace(vect2, vgrps2);

	//overlap.couplingGraphs(vgrps1, vgrps2);
	
	return 0;

	/**/

	//printf("-----------------------------------------------\n");

	//overlap.calcMainArea(vect1, rect);

	/*BoxShape * sh = vshap1.at(1);
	overlap.printShape(*sh);*/

	/*vector<geometry::Rect*> vect2;
	result = LoadRectangles("C:\\Imagens\\Matriz\\Proto5.png", vect2);
	vector<BoxShape*> vshap2;
	overlap.loadContainers(vect2, vshap2);*/
	/*sh = vshap2.at(1);
	overlap.printShape(*sh);*/

	/*double factor = 0;
	overlap.calcScaleFactor(vshap1, vshap2, &factor);*/
	//printf("Factor: %.12f \n", factor);

	//overlap.verifyAffinity(vshap1, vshap2, rect);

	//RRect rc1;
	//vector<geometry::Rect*> vect1;
	//result = LoadRectangles("C:\\Imagens\\Matriz\\Proto1.png", vect1);
	////overlap.xuxuBeleza(vect1);
	//overlap.calculeExtremes(vect1, rc1);
	//printf("Rect_A: Width: %.4f - Height: %.4f \n", rc1.width, rc1.height);

	//printf("----------------------------------------------\n");

	//RRect rc2;
	//vector<geometry::Rect*> vect2;
	//result = LoadRectangles("C:\\Imagens\\Matriz\\Proto5.png", vect2);  // Proto4
	////overlap.xuxuBeleza(vect2);
	//overlap.calculeExtremes(vect2, rc2);
	//printf("Rect_B: Width: %.4f - Height: %.4f \n", rc2.width, rc2.height);

	//overlap.createTarget(vect, NULL);


	//Mat src = imread("C:\\Imagens\\Matriz\\Novas\\01113304.jpg");
	//if (src.empty())
	//{
	//	cout << "Could not open or find the image!\n" << endl;
	//	waitKey();
	//	return -1;
	//}

	//RNG rng(12345);
	//Mat gray, timg;

	//cvtColor(src, gray, CV_BGR2GRAY);
	////threshold(gray, timg, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
	//threshold(gray, timg, 150, 255, THRESH_BINARY);

	//vector<vector<cv::Point>> contours;
	//vector<Vec4i> hierarchy;

	//findContours(timg, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	//vector<vector<cv::Point>> contours_poly(contours.size());
	//vector<cv::Rect> boundRect(contours.size());

	//Mat drawing;
	//src.copyTo(drawing);

	//for (int i = 0; i < contours.size(); i++)
	//{
	//	approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
	//	boundRect[i] = boundingRect(Mat(contours_poly[i]));
	//	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
	//}

	//imwrite("C:\\Imagens\\Reposit\\Region_01113304.jpg", drawing);


	/*Mat element = getStructuringElement(MORPH_CROSS, cv::Size(3, 3)) * 255;
	imwrite("C:\\Imagens\\Kernel.png", element);
	waitKey(0);*/



	//Analiser analiser;
	////string filename = "C:\\Imagens\\Matriz\\883912390.jpg";
	////string filename = "C:\\Imagens\\Matriz\\8784.jpg";
	////string filename = "C:\\Imagens\\Matriz\\091099.png";
	////string filename = "C:\\Imagens\\Matriz\\154357150.png";
	////string filename = "C:\\Imagens\\Matriz\\26865.jpg";
	////string filename = "C:\\Imagens\\Matriz\\56266524.jpg";
	////string filename = "C:\\Imagens\\Matriz\\117457.png";
	//string filename = "C:\\Imagens\\Matriz\\105314.png"; //
	////string filename = "C:\\Imagens\\Matriz\\INIT_MINIMUM.png";
	////string filename = "C:\\Imagens\\Matriz\\114517365.png";
	////string filename = "C:\\Imagens\\Matriz\\132211.png";
	//analiser.ProcessImage(filename.c_str());
	
	/*double mat[] { *(graph.getAdjacencies()) };
	double dst[] { 0.00 };
	graph.reduceMatrix(mat, 4, dst);*/

	/*double * mat = graph.getAdjacencies();
	double * res = NULL;
	res = graph.transformToMatrix(mat, 4, 1, 1);
	graph.printMatrix(res, 3);*/
	//res = graph.transformToMatrix(mat, 4, 2, 2);
	//graph.printMatrix(res, 2);

	//graph.printMatrix();

	/*graph.printMatrix();
	printf("\n\n");
	graph.calcSpectrum();
	printf("\n\n");
	graph.printMatrix();*/
	
	return 0;

	//Mat src = imread("C:\\Imagens\\Matriz\\Proto1.png");
	//if (src.empty())
	//{
	//	cout << "Could not open or find the image!\n" << endl;
	//	waitKey();
	//	return -1;
	//}

	//RNG rng(12345);
	//Mat gray, timg;

	//cvtColor(src, gray, CV_BGR2GRAY);
	////threshold(gray, timg, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
	//threshold(gray, timg, 150, 255, THRESH_BINARY);

	//vector<vector<cv::Point>> contours;
	//vector<Vec4i> hierarchy;

	//findContours(timg, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	//vector<vector<cv::Point>> contours_poly(contours.size());
	//vector<cv::Rect> boundRect(contours.size());

	//Mat drawing;
	//src.copyTo(drawing);

	//for (int i = 0; i < contours.size(); i++)
	//{
	//	approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
	//	boundRect[i] = boundingRect(Mat(contours_poly[i]));
	//}

	//waitKey();

	//return 0;
}


// https://gist.github.com/marcoscastro/76634e1d38d0d7eef2e1