#ifndef _THRESH_
#define _THRESH_
#include "Header.h"

double otsu(const Mat & img);
Mat createGausFilterMask(Size mask_size, int x, int y, int ksize, bool normalization, bool invert);
Mat kernel3();
#endif //_THRESH_
