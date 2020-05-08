#include "CharExtract.h"

std::pair<Point, Point>		bfs(Mat &src, Point start, uint8_t xVal = 255, uint8_t newVal = 0) {
	Point Min = Point(src.rows-1, src.cols-1);
	Point Max = Point(0, 0);
	Mat tmp = Mat(src.size(), CV_8UC1, Scalar(0));
	vector<Point> points;
	points.resize(src.rows * src.cols);
	int top = 1;

	if (pxu(src,start.x, start.y) == xVal) {
		points[0] = start;
		pxu(tmp, start.x, start.y) = 1;
		pxu(src, start.x, start.y) = newVal;
		if (start.x < Min.x) Min.x = start.x;
		if (start.y < Min.y) Min.y = start.y;
		if (start.x > Max.x) Max.x = start.x;
		if (start.y > Max.y) Max.y = start.y;
	}

	while (top) {
		Point point = points[--top];
		
		for (int i = -1; i <= 1; ++i)
			for (int j = -1; j <= 1; ++j)
				if (abs(i) + abs(j) == 1) {
					Point nxt = Point(point.x + i, point.y + j);
					if (nxt.x >= 0 && nxt.x < src.rows && nxt.y >= 0 && nxt.y <= src.cols) {
						if (pxu(tmp, nxt.x, nxt.y) == 0 && pxu(src, nxt.x, nxt.y) == xVal) {
							points[top++] = nxt;
							pxu(tmp, nxt.x, nxt.y) = 1;
							pxu(src, nxt.x, nxt.y) = newVal;
							if (nxt.x < Min.x) Min.x = nxt.x;
							if (nxt.y < Min.y) Min.y = nxt.y;
							if (nxt.x > Max.x) Max.x = nxt.x;
							if (nxt.y > Max.y) Max.y = nxt.y;
						}
					}
				}
	}
	points.resize(0);
	return  std::make_pair(Min, Max);
}

vector<int>	 calSumCol(const Mat &img) {
	Mat src = img.clone();
	vector<int> sumCol;
	int vSize = 150; //>= 100
	int hthres = 50;
	int mthres = 30;
	int lthres = 10;
	int ha, ma, la;
	ha = ma = la = 0;

	Mat visualize = Mat(vSize, src.cols, CV_8UC3, Scalar(255, 255, 255));
	sumCol.resize(src.cols);
	for (int j = 0, row = src.rows, col = src.cols; j < col; ++j) {
		sumCol[j] = sum(src(Rect(j, 0, 1, row)))[0] / 255;
		int t = min(100, max(0, sumCol[j] * 100 / row)) * (vSize/100.0);
		visualize(Rect(j, vSize-t, 1, t)) = Scalar(255, 100, 50);
		if (t < lthres) 
			src(Rect(j, 0, 1, row)) = 100;
	}

	Scalar colorLine = Scalar(0, 0, 255);
	visualize(Rect(0, vSize-lthres*(vSize/100.0), src.cols, (vSize/100.0))) = colorLine; //25%
	//visualize(Rect(0, vSize-mthres*(vSize/100.0), src.cols, (vSize/100.0))) = colorLine; //50%
	//visualize(Rect(0, vSize-hthres*(vSize/100.0), src.cols, (vSize/100.0))) = colorLine; //75%
	imshow("visualize sum of intensity in column", visualize);
	imshow("apply threshold (red line)", src);
	return sumCol;
}

Mat 	CE::extractChar(const Mat &img) {
	Mat src;
	cvtColor(img, src, CV_BGR2GRAY);
	resize(src, src, src.size() * 2);

	//Mat canny;
	//Canny(src, canny, 10, 200);
	
	//adaptiveThreshold(canny, canny, 1, ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 15, 5);
	//imshow("canny", canny);
	//multiply(src, canny, src);

	adaptiveThreshold(src, src, 255, ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 15, 5);
	imshow("local threshold", src);
	bfs(src, Point(0, 0));
	bfs(src, Point(src.rows-1, 0));
	bfs(src, Point(0, src.cols-1));
	bfs(src, Point(src.rows-1, src.cols-1));
	imshow("extract text", src);
	
	//Mat slide = src.clone();
	vector<int> sumCol = calSumCol(src);
	
	//imshow("map", map);
	//imshow("res", src);
	return src;
}
/*
	Mat map;
	cvtColor(src, map, CV_GRAY2BGR);
	
	for (int i = 0, row = src.rows; i < row; ++i)
		for (int j = 0, col = src.cols; j < col; ++j) {
			if (pxu(src, i, j) == 255){
				pair<Point, Point> p = bfs(slide, Point(i, j), 255, 100);
				Point tl = Point(p.first.y, p.first.x);
				Point br = Point(p.second.y, p.second.x);
				rectangle(map, tl, br, Scalar(0, 255, 0), 1, 8, 0);
			}
		}
	imshow("characters", map);
	return map;
*/

	//Mat tmp;
	//double thres = otsu(img);
	//threshold(src, tmp, thres, 1, CV_8UC1);
//	imshow("threshold", tmp);
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
//	erode(tmp, tmp, element);
//	imshow("erode", tmp);

	/*
	Mat mask = Mat(src.size(), CV_8UC1, Scalar(0));
	Mat border;
	Canny(src, border, 100, 200);
	copyMakeBorder(border, border, 1, 1, 1, 1, cv::BORDER_REPLICATE);
	floodFill(mask, border, Point(mask.cols/2, mask.rows/2), Scalar(255), 0, Scalar(), Scalar());
	imshow("mask", border);
	*/
	//multiply(src, tmp, src);
	//normalize(src, src, 0, 255, CV_MINMAX, CV_8UC1);
	//Canny(src, src, 80, 200, 3);
	//element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
	//imshow("canny", src);
	//Mat kernel = Mat(3,3, CV_8UC1);
	//pxu(kernel, 1, 0) = pxu(kernel, 1, 1) = pxu(kernel, 1, 2) = 1;
	//dilate(src, src, kernel);
	//imshow("dilate", src);
	
	//Create a mask from edges in the original image
	
	/*Mat mask;
	Canny(src, mask, 100, 150);
	copyMakeBorder(mask, mask, 1, 1, 1, 1, cv::BORDER_REPLICATE);
	imshow("mask", mask);
	Fill mask with value 128
	
	uchar fillValue = 0;
	floodFill(src, mask, Point(0, 0), Scalar(0), 0, Scalar(), Scalar());
	floodFill(src, mask, Point(src.cols-1, src.rows-1), Scalar(0), 0, Scalar(), Scalar());
	floodFill(src, mask, Point(0, src.rows-1), Scalar(0), 0, Scalar(), Scalar());
	floodFill(src, mask, Point(src.cols-1, 0), Scalar(0), 0, Scalar(), Scalar());
	*/
