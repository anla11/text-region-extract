#include "process.h"

double	otsu(const Mat &src) {
	Mat img;
	if (src.channels() != 1)
        cvtColor(src, img, CV_BGR2GRAY);
	else
		img = src.clone();
	normalize(img, img, 0, 255, CV_MINMAX, CV_8UC1);

	int histSize = 256;
    Mat his;
    double mean[256];
    double Pr[256];
    double sum[256];

    int row = img.rows;
    int col = img.cols;

	/// Set the ranges 
	float range[] = { 0, 256 } ;
	const float* histRange = { range };
	calcHist( &img, 1, 0, Mat(), his, 1, &histSize, &histRange, true, false);

    sum[0] = his.at<float>(0);
    mean[0] = 0;

    his.at<float>(0) /= (row * col);
    for (int i = 1; i < 256; ++i)
    {
        his.at<float>(i) /= (row * col);
        sum[i] = sum[i - 1] + his.at<float>(i);
        mean[i] = mean[i - 1] + i * his.at<float>(i);
    }

    double variance = -1;
    double res = 0;
    for (int i = 1; i < 255; ++i)
    {
        Pr[i] = sum[i] / sum[255];
        double tu = mean[255] * Pr[i] - mean[i];
        double mau = Pr[i] * (1 - Pr[i]);
        if (variance < tu / mau * tu)
        {
            variance = tu / mau * tu;
            res = i;
        }
    }
	if (src.type() == CV_32FC1 || src.type() == CV_32FC3)
		return res/255;
    return res;
}

Mat createGausFilterMask(Size mask_size, int x, int y, int ksize, bool normalization, bool invert) {
    // Some corrections if out of bounds
    if(x < (ksize / 2)) {
        ksize = x * 2;
    }
    if(y < (ksize / 2)) {
        ksize = y * 2;
    }
    if(mask_size.width - x < ksize / 2 ) {
        ksize = (mask_size.width - x ) * 2;
    }
    if(mask_size.height - y < ksize / 2 ) {
        ksize = (mask_size.height - y) * 2;
    }
 
    // call openCV gaussian kernel generator
    double sigma = -1;
    Mat kernelX = getGaussianKernel(ksize, sigma, CV_32F);
    Mat kernelY = getGaussianKernel(ksize, sigma, CV_32F);
    // create 2d gaus
    Mat kernel = kernelX * kernelY.t();
    // create empty mask
    Mat mask = Mat::zeros(mask_size, CV_32F);
    Mat maski = Mat::zeros(mask_size, CV_32F);
 
    // copy kernel to mask on x,y
    Mat pos(mask, Rect(x - ksize / 2, y - ksize / 2, ksize, ksize));
    kernel.copyTo(pos);
 
    // create mirrored mask
    Mat posi(maski, Rect(( mask_size.width - x) - ksize / 2, (mask_size.height - y) - ksize / 2, ksize, ksize));
    kernel.copyTo(posi);
    // add mirrored to mask
    add(mask, maski, mask);
 
    // transform mask to range 0..1
    if(normalization) {
        normalize(mask, mask, 0, 1, NORM_MINMAX);
    }
    // invert mask
    if(invert) {
        mask = Mat::ones(mask.size(), CV_32F) - mask;
    }
    return mask;
}


// hàm phụ
Mat		kernel3() {
	Mat kernel = Mat(3, 3, CV_8UC1, Scalar_<uint8_t>(0));
	pxu(kernel, 0, 1) = pxu(kernel, 1, 0) = pxu(kernel,1,1) = pxu(kernel, 1,2) = pxu(kernel, 2,1) = 1;
	return kernel;
}

