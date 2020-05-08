#ifndef _EDGEDRAWING_
#define _EDGEDRAWING_
#include "Header.h"

using namespace std;
using namespace cv;

namespace ED {
	int checkPixel(const pair<int, int> &pos, const Size &size);
	Mat	findEdge(const Mat &gray, double thes1, double thes2, int step);
}
#endif //_EDGEDRAWING_