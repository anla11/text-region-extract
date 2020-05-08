#ifndef _TRE_
#define _TRE_

#include "Header.h"
#include "morphology.h"
#include "process.h"
#include "myFFT.h"

using namespace myFFT;
using namespace MORPH;

namespace TRE {
	vector<Rect>	tre1(const Mat &img);
	vector<Rect>	tre2(const Mat &img);
	//vector<Rect>	tre3(const Mat &img);
	//vector<Rect>	findTextBox(const Mat&img, const Mat&mask);
}
#endif //_TRE_