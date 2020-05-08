#ifndef _HEADER_
#define _HEADER_

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv_modules.hpp>

#define pxu(img, x, y) img.at<uint8_t>(x, y)
#define pxf(img, x, y) img.at<float>(x, y)

using namespace std;
using namespace cv;

const string DEFAULT_INPUT = "Input/";
const string DEFAULT_VIDE0 = "";

#endif //_HEADER_