#include "TextRegionExtract.h"


Mat element_5 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(2, 2));
Mat element_3 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));

//TRE - Text Region Extract


vector<Rect>	findTextBox(const Mat &img, const Mat & mask) {
	Mat src = img.clone();
	vector<vector<Point> > contours;
	findContours(mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	vector<Rect> rects;
	for (int i = 0, sz = contours.size(); i < sz; ++i) {
		rects.push_back(boundingRect(contours[i]));
	}
	return rects;
}

vector<Rect>	TRE::tre1(const Mat &img) {
	//Step 0
	Mat src;
	cvtColor(img, src, CV_BGR2GRAY);

	//Step 1
	Mat element;
	element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(2,2));
	Mat open = opening(src, element_5);
	Mat close = closing(src, element_5);
	Mat diff;
	absdiff(open, close, diff);
	//imshow("diff", diff);

	//Step 2
	Mat lowpass;
	lowPass(diff, lowpass, 8);
	//imshow("Low Pass", lowpass);

	//Step 3
	Mat thresMat;
	double thres = otsu(lowpass);
	threshold(lowpass, thresMat, thres, 1, THRESH_BINARY);
	normalize(thresMat, thresMat, 0, 1, CV_MINMAX, CV_8UC1);
	

	for (int i = 0; i < 3; ++i)
		dilate(thresMat, thresMat, element_5);
	for (int i = 0; i < 2; ++i)
		erode(thresMat, thresMat, element_3);

	//imshow("Otsu", thresMat);

	//Mat res;
	//multiply(src, thresMat, res);
	vector<Rect> rects = findTextBox(img, thresMat);

	return rects;
}

vector<Rect>	TRE::tre2(const Mat &img) {
	Mat src;
	cvtColor(img, src, CV_BGR2GRAY);

	Mat open = opening(src, element_5);
	Mat close = closing(src, element_5);
	absdiff(open, close, src);

	double thres = otsu(src);
	threshold(src, src, 1.75 * thres, 1, THRESH_BINARY);

	for (int i = 0; i < 3; ++i)
		dilate(src, src, element_5);
	for (int i = 0; i < 2; ++i)
		erode(src, src, element_3); 

	/*
	Mat view;
	normalize(src, view, 0, 255, CV_MINMAX, CV_8UC1);
	imshow("view", view);
	*/
	/*Mat res;
	multiply(img, src, res);*/

	vector<Rect> rects = findTextBox(img, src);

	return rects;
}


// kết quả sai
/*
Mat		TRE::tre3(const Mat &gray) {
	Mat grx, gry, gr, dilation, thresMat, edge;
	Sobel(gray, grx, CV_8UC1, 0, 1, 3);
	Sobel(gray, gry, CV_8UC1, 1, 0, 3);
	gr = abs(grx) + abs(gry);
	normalize(gr, gr, 0, 255, CV_MINMAX, CV_8UC1);
	blur(gr, gr, Size(5, 5));
	//Canny(gr, edge, 100, 200, 3);
	//threshold(edge, edge, 50, 1, CV_8UC1);
	//multiply(gr, edge, thresMat);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
	
	dilate(gr, dilation, element);
	//element = getStructuringElement(MORPH_CROSS, Size(3, 3), Point(1, 1));
	//morphologyEx(dilation, edge, MORPH_OPEN, element);
	return dilation;
}
*/