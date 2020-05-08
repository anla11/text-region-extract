#include "../MyLib/Header.h"
#include "../MyLib/process.h"

int main() {
	string path = DEFAULT_INPUT + "cameraman.jpg";
	Mat img = imread(path.c_str());

	double thres = otsu(img);
	cout << thres;
	threshold(img, img, thres, 255, CV_8UC1);
	imshow("img", img);
	waitKey(0);
	return 0;
}