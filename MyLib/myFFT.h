#ifndef _myFFT_
#define _myFFT_

#include "Header.h"
#include "processFrq.h"
#include <complex>

typedef complex<float> cplx;
const double PI = acos(-1);

namespace	myFFT
{
	const int IDEAL = 1;
	const int BUTTER = 2;
	const int GAUSS = 3;

	void    filter(Mat frqSrc, Mat &frqDst, float thrhd, int type);
	// frqSrc is FFT value of Image, 2 channels: real (fr), imag (fim)
	// frqDst is FFT filterd value of Image, 2 channels: real (r), imag (im)
	//void	filterPass(const Mat &src,  Mat &dst, double thres, int type = GAUSS);
	void	lowPass(const Mat &src,  Mat &dst, double thres, int type = GAUSS);
	void	highPass(const Mat &src, Mat &dst, double thres, int type = GAUSS);
	void	filterPass(const Mat &src, Mat &dst, int size, int type);	//laplace filter

	void	fft(const Mat &src, Mat &r, Mat &im, int inverse = -1);
	void    fft(const Mat &fr, const Mat &fim, Mat &r, Mat &im, int inverse = -1);
	//input of FFT (or inverse FFT) can be Mat_<float> or Mat_<float, float>

}
#endif // _myFFT_
