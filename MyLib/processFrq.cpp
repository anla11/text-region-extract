#include "processFrq.h"


int     find2Pow(int n)
{
    int pwr = 1, tmp = n;
    while (tmp != 1){
        n += (tmp % 2) * pwr;
        tmp += (tmp % 2);
        pwr <<= 1;
        tmp >>= 1;
    }
    return n;
}

void    myrotate(const Mat &src, Mat& dst, int angle)
{
    Point2f src_center(0, 0);
    Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
    if ((angle / 90) % 2 == 0)
        warpAffine(src, dst, rot_mat, src.size());
    else
    {
        Size dst_size= Size(src.rows, src.cols);
        warpAffine(src, dst, rot_mat, dst_size);
    }
}

void    rotate_FromCenter(Mat &magI){
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void    recoverImage(Mat r, Mat im, Mat &img){
    Mat magI;
    magnitude(r, im, magI);
    normalize(magI, img, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
}

void    recoverImage(Mat complexI, Mat &img){
    vector<Mat> planes;
    split(complexI, planes);
    Mat magI;
    magnitude(planes[0], planes[1], magI);
    normalize(magI, img, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
}

void    findSpecTrum(Mat r, Mat im, Mat &sptr)
{
    Mat magI;
    magnitude(r, im, magI);
    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);
    normalize(magI, sptr, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
}

void    findSpecTrum(Mat complexI, Mat &sptr)
{
    vector<Mat> planes;
    split(complexI, planes);
    findSpecTrum(planes[0], planes[1], sptr);

    normalize(sptr, sptr, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
}