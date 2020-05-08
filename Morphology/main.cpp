#include "../MyLib/morphology.h"

using namespace MORPH;

int main(){
	string path = DEFAULT_INPUT + string("maru.jpg");
	Mat img = imread(path.c_str());
	cvtColor(img, img, CV_BGR2GRAY);
	imshow("image", img);
	Mat kernel = Mat(Size(3,3), CV_8UC1, Scalar_<uint8_t>(0));
	pxu(kernel, 1, 1) = pxu(kernel, 0, 1) = pxu(kernel, 1, 0) = pxu(kernel, 1, 2) = pxu(kernel, 2, 1) = 1;
	Mat tmp = closing(img, kernel);
	imshow("my res", tmp);
	waitKey(0);
	return 0;
}