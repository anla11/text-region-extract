#ifndef _PROCESS_FRQ_
#define _PROCESS_FRQ_

#include "header.h"

using namespace std;
using namespace cv;

int     find2Pow(int n);
void    myrotate(const Mat &src, Mat &dst, int angle);	//rotate angle = k * 90 (k: interger)
void    rotate_FromCenter(Mat &magI);
void    findSpecTrum(Mat r, Mat im, Mat &sptr);
void    findSpecTrum(Mat complexI, Mat &sptr);
void    recoverImage(Mat r, Mat im, Mat &img);
void    recoverImage(Mat complexI, Mat &img);


#endif //_PROCESS_FRQ