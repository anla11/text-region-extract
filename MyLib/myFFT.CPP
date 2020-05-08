#include "myFFT.h"

void    myFFT::filter(Mat frqSrc, Mat &frqDst, float thrhd, int type)
{
	 type = -type;
	 if ((type == 2 && thrhd == 0) || (type == 3 && thrhd == 0))
     {
          frqDst = Mat(frqSrc.size(), CV_32FC2, Scalar_<float>(0, 0));
          return;
     }
	 if (type == -3 && thrhd == 0)
	 {
		 frqDst = frqSrc.clone();
		 return;
	 }

     int row = frqSrc.rows, col = frqSrc.cols;
     int cx = row/2, cy = col /2;
     frqDst = frqSrc.clone();
    

     for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
        {
            float d = sqrt((i-cx)*(i-cx) + (j-cy)*(j-cy));

            switch (abs(type))
            {
                case IDEAL: 
                    if ((type < 0 && d < thrhd) || (type > 0 && d > thrhd))
                       frqDst.at<Vec2f>(i, j)[0] = frqDst.at<Vec2f>(i, j)[1] = 0;
				break;
                case BUTTER:
                    if (type > 0)
                        frqDst.at<Vec2f>(i, j)[0] = frqSrc.at<Vec2f>(i, j)[0] * 1.0 /(1 + pow(d/thrhd, 2*4)),
                        frqDst.at<Vec2f>(i, j)[1] = frqSrc.at<Vec2f>(i, j)[1] * 1.0 /(1 + pow(d/thrhd, 2*4));
                    if (type < 0 && d)
                        frqDst.at<Vec2f>(i, j)[0] = frqSrc.at<Vec2f>(i, j)[0] * 1.0 /(1 + pow(thrhd/d, 2*4)),
                        frqDst.at<Vec2f>(i, j)[1] = frqSrc.at<Vec2f>(i, j)[1] * 1.0 /(1 + pow(thrhd/d, 2*4));

                    break;
                case GAUSS:
                    float tmp = exp(-d*d/(2 * thrhd * thrhd));
                    if (type > 0)
                        frqDst.at<Vec2f>(i, j)[0] = frqSrc.at<Vec2f>(i, j)[0] * tmp,
                        frqDst.at<Vec2f>(i, j)[1] = frqSrc.at<Vec2f>(i, j)[1] * tmp;
                    if (type < 0)
                        frqDst.at<Vec2f>(i, j)[0] = frqSrc.at<Vec2f>(i, j)[0] * (1 - tmp),
                        frqDst.at<Vec2f>(i, j)[1] = frqSrc.at<Vec2f>(i, j)[1] * (1 - tmp);
                    break;
            }
        } 
}
//fft 1D
void    _fft(vector<cplx> &buf, vector<cplx> &out, int n, int step, int inverse){
    if (step >= n)
        return;
    _fft(out, buf, n, step * 2, inverse);

    for (int i = 0; i < n/2; ++i)
    {
        //sum = i, sub = i + n/2, sum = left + j * step
        int left = i % step;
        int j = (i-left)/step;
        int pos = left + step * 2 * (j % (n/2/step));
        
		// pos1 = pos,  pos2 = pos1 + step;
        // cplx F =  exp(-I * PI * j* step * 2/ n);
        float r = cos(inverse *PI * j * step * 2 /n);
        float im = sin(inverse *PI *  j *step * 2/n);
        cplx F = cplx(r, im);
        F = F * out[pos + step];

        buf[i] = out[pos] + F;
        buf[i + n/2] = out[pos] - F;
    }
}

void    myFFT::fft(const Mat &img, Mat &r, Mat &im, int inverse){
    Mat src;
    if (img.channels() > 2)
        cvtColor(img, src, CV_BGR2GRAY);
    else
        src = img.clone();
	src.convertTo(src, CV_32F);

    int row = find2Pow(src.rows), col = find2Pow(src.cols);

    resize(src, src, Size(col, row));
    r = Mat(src.size(), CV_32FC1);
    im = Mat(src.size(), CV_32FC1);

    vector<cplx> src_row, tmp;
	vector<float> r_row, im_row;
    src_row.resize(col);
	tmp.resize(col);
    r_row.resize(col);
    im_row.resize(col);

    for (int i = 0; i < row; ++i)
    {
		for (int j = 0; j < col; ++j)
		{
			if (inverse == -1)
				src_row[j] = cplx(src.at<float>(i, j), 0);
			else
				src_row[j] = cplx(src.at<Vec2f>(i, j)[0], src.at<Vec2f>(i, j)[1]);
			tmp[j] = src_row[j];
		}
        _fft(src_row, tmp, col, 1, inverse);

        for (int j = 0; j < col; ++j)
            r.at<float>(i, j) = src_row[j].real(),
            im.at<float>(i, j) = src_row[j].imag();
    }	

    vector<cplx> buf, out;
    buf.resize(row);
    out.resize(row);

    for (int j = 0; j < col; ++j)
    {
        for (int i = 0; i < row; ++i)
        {
            buf[i] = cplx(r.at<float>(i, j), im.at<float>(i, j));
            out[i] = buf[i];
        }
        _fft(buf, out, row, 1, -1);
        for (int i = 0; i < row; ++i)
            r.at<float>(i, j) = buf[i].real(),
            im.at<float>(i, j) = buf[i].imag();
    }

    resize(r, r, img.size());
    resize(im, im, img.size());

	if (inverse == -1)
		rotate_FromCenter(r), rotate_FromCenter(im);
	else
		flip(r, r, 0), flip(im, im, 0);
}

void    myFFT::fft(const Mat &fr, const Mat &fim, Mat &r, Mat &im, int inverse){
	Mat complexI;
	vector<Mat> planes;
	planes.push_back(fr);
	planes.push_back(fim);
	merge(planes, complexI);
	myFFT::fft(complexI, r, im, inverse);
}

void	myFFT::filterPass(const Mat &img, Mat &dst, int size, int type) {
	Mat src = img.clone();
	if (src.channels() > 1)
        cvtColor(src, src, CV_BGR2GRAY);
    Mat fImg, frq, filtered;
    src.convertTo(fImg, CV_32F);
    cv::dft(fImg, frq, cv::DFT_COMPLEX_OUTPUT);
    rotate_FromCenter(frq);
	filter(frq, filtered, size, type);
	dft(filtered, frq, cv::DFT_INVERSE);
    recoverImage(frq, dst);
}

/*
void	myFFT::filterPass(const Mat &img, Mat &dst, double thrhd, int type) {
	Mat src = img.clone();
	if (src.channels() > 1)
        cvtColor(src, src, CV_BGR2GRAY);
    Mat fImg, frq, filtered;
    src.convertTo(fImg, CV_32F);
    cv::dft(fImg, frq, cv::DFT_COMPLEX_OUTPUT);
    rotate_FromCenter(frq);
	filter(frq, filtered, thrhd, type);
	dft(filtered, frq, cv::DFT_INVERSE);
    recoverImage(frq, dst);
}
*/
void	myFFT::lowPass(const Mat &src, Mat &dst, double thrhd, int type) {
	myFFT::filterPass(src, dst, thrhd, -type);
}

void	myFFT::highPass(const Mat &src, Mat &dst, double thrhd, int type) {
	myFFT::filterPass(src, dst, thrhd, type);
}