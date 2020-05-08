#include "morphology.h"

namespace MORPH {
	Mat		dilation(const Mat &src, const Mat &kernel, Point center){
		/*
		 my code - run in release mode
		int rowk = kernel.rows;
		int colk = kernel.cols;
		if (center == Point(-1, -1))
			center = Point(rowk/2, colk/2);
		Mat res = Mat(src.size(), CV_8UC1, Scalar_<uint8_t>(0));
		for (int i = center.x, row = src.rows - (rowk - center.x); i < row; ++i)
			for (int j = center.y, col = src.cols - (colk - center.y); j < col; ++j) {
				if (pxu(src, i, j) > 0) {
					uint8_t Max = 0;
					for (int x = 0; x < rowk; ++x)
						for (int y = 0; y < colk; ++y) {
							int tmpx = i + x - center.x;
							int tmpy = j + y - center.y;
							if (pxu(kernel, x, y) != 0 && min(255, pxu(src, tmpx, tmpy) + pxu(kernel, x, y)) > Max)
								Max = min(255, pxu(src, tmpx, tmpy) + pxu(kernel, x, y));
						}
					for (int x = 0; x < rowk; ++x)
						for (int y = 0; y < colk; ++y) {
							int tmpx = i + x - center.x;
							int tmpy = j + y - center.y;
							if (pxu(kernel, x, y) != 0)
								pxu(res, tmpx, tmpy) = Max;
						}
				}
			}
		return res; 
		*/
		Mat lib;
		dilate(src, lib, kernel);
		return lib;
	}


	Mat		erosion(const Mat &src, const Mat &kernel, Point center){
		/*
		/ my code - run in release mode

		int rowk = kernel.rows;
		int colk = kernel.cols;
		if (center == Point(-1, -1))
			center = Point(rowk/2, colk/2);
		Mat res = src.clone();
		for (int i = center.x, row = src.rows - (rowk - center.x); i < row; ++i)
			for (int j = center.y, col = src.cols - (colk - center.y); j < col; ++j) {
				if (pxu(src, i, j) > 0) {
					uint8_t Min = 255;
					for (int x = 0; x < rowk; ++x)
						for (int y = 0; y < colk; ++y) {
							int tmpx = i + x - center.x;
							int tmpy = j + y - center.y;
							if (pxu(kernel, x, y) != 0 && max(0, pxu(src, tmpx, tmpy) - pxu(kernel, x, y)) < Min) 
								Min = max(0, pxu(src, tmpx, tmpy) - pxu(kernel, x, y)) ;
						}
					for (int x = 0; x < rowk; ++x)
						for (int y = 0; y < colk; ++y) {
							int tmpx = i + x - center.x;
							int tmpy = j + y - center.y;
							if (pxu(kernel, x, y) != 0)
								pxu(res, tmpx, tmpy) = Min;
						}
				}
			}
		return res;
		*/
		Mat lib;
		erode(src, lib, kernel);
		return lib;
	}

	Mat		opening(const Mat &src, const Mat &kernel, Point center) {
		Mat res = erosion(src, kernel, center);
		res = dilation(res, kernel, center);
		return res;
		/*
		Mat lib;
		morphologyEx(src, lib, MORPH_OPEN, kernel);
		return lib;
		*/
	}

	Mat		closing(const Mat &src, const Mat &kernel, Point center) {
		Mat res = dilation(src, kernel, center);
		res = erosion(res, kernel, center);
		return res;	
		/*
		Mat lib;
		morphologyEx(src, lib, MORPH_CLOSE, kernel);
		return lib;
		*/	
	}
}