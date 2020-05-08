#ifndef _MORPHOLOGY_
#define _MORPHOLOGY_
#include "Header.h"

namespace MORPH {
	Mat dilation(const Mat &src, const Mat &kernel, Point center = Point(-1, -1));
	Mat erosion(const Mat &src, const Mat &kernel, Point center = Point(-1, -1));
	Mat opening(const Mat &src, const Mat &kernel, Point center = Point(-1, -1));
	Mat closing(const Mat &src, const Mat &kernel, Point center = Point(-1, -1));
}
#endif //_MORPHOLOGY_