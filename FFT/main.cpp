#include "../MyLib/Header.h"
#include "../MyLib/myFFT.h"

using namespace myFFT;



int main() {
	string path = DEFAULT_INPUT + string("maru.jpg");
	Mat img = imread(path.c_str());
	Mat res;
	//filterPass(img, res, 5, 1);
	//imshow("res", res);
	lowPass(img, res, 20, IDEAL);
	imshow("low", res);
	//highPass(img, res, 20);
	//imshow("high", res);
	waitKey(0);
	return 0;
}