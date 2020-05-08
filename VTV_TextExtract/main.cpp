#include "../MyLib/Header.h"
#include "../MyLib/EdgeDrawingAlg.h"
#include "../MyLib/TextRegionExtract.h"
#include "CharExtract.h"
#include <time.h>

using namespace TRE;
using namespace MORPH;
using namespace myFFT;


//TRE
// algorithm 1: kết quả tương đối chính xác, khung viền quá to so với chữ, tốc độ chậm. (tốc độ ~0.160 )
// algorithm 2: kết quả chính xác, vừa vặn với chữ, tốc độ ~0.010

//threshold to filter result
const int RATIO_THRES = 5; //width / height
const int MIN_HEIGHT = 20;
const int MAX_HEIGHT = 130;
const int POS_HEIGHT = 350;

const int VISUAL_DEBUG = 1;

int		filterTextBox(Rect rect) {
	return (rect.height > MIN_HEIGHT && rect.height < MAX_HEIGHT && rect.width/rect.height > RATIO_THRES);
}

Mat		drawTextBox(const Mat&img, vector<Rect> rects) {
	Mat src = img.clone();
	for (int i = 0, sz= rects.size(); i < sz; ++i) {
		if (filterTextBox(rects[i]))
			rectangle(src, rects[i].tl(), rects[i].br(), Scalar(0, 255, 0), 2, 8, 0);
	}
	return src;
}

vector<Mat>		extractTextBox(const Mat& img, const vector<Rect> &rects){
	vector<Mat> textboxes;
	for (int i = 0, sz = rects.size(); i < sz; ++i) {
		if (filterTextBox(rects[i]))
			textboxes.push_back(img(rects[i]));
	}
	return textboxes;
}

void	extractChar(vector<Mat> &chars, const Mat&img, const vector<Mat> &textboxes) {
	for (int j = 0, sz = textboxes.size(); j < sz; ++j){
		Mat tmp = textboxes[j];
		tmp = CE::extractChar(tmp);
		//chars.push_back(tmp);
	}
}

int		readVideo(const char* path){
	VideoCapture cap(path);
	if (!cap.isOpened()) {
		cout << "Cannot read video.\n";
		return 0;
	}
	//get properties of video
	int nframe = (int) cap.get(CV_CAP_PROP_FRAME_COUNT);
	int fps = (int) cap.get(CV_CAP_PROP_FPS);

	clock_t s1, s2, f1, f2, t1 = 0, t2 = 0;
	vector<Rect> rects;
	vector<Mat> textboxes;
	vector<Mat> chars;
	chars.resize(0);

	int i;
	for (i = 0; i < nframe; ++i) {
		Mat frame;
		cap >> frame;
		imshow("frame", frame);
		if (i % fps == 0)
		{
			/*
			s1 = clock();
			rects = tre1(frame);
			f1 = clock();
			textboxes = extractTextBox(frame, rects);
			
			Mat res1 = drawTextBox(frame, rects);
			imshow("Algorithm 1", res1);
			t1 += f1 - s1;
			*/

			s2 = clock();
			rects  = tre2(frame);
			textboxes = extractTextBox(frame, rects);
			f2 = clock();

			//Mat res2 = drawTextBox(frame, rects);
			//imshow("Algorithm 2", res2);
			for (int j = 0, sz = textboxes.size(); j < sz; ++j) {
				char name[50];
				sprintf(name, "Output/%d_%d.png\0", i, j);
				printf("%s\n", name);
				imwrite(name, textboxes[j]);
				//imshow(name, textboxes[j]);
			}
			
			//extractChar(chars, frame, textboxes);

			t2 += f2 - s2;
		}
		if (waitKey(1) == 27)
			break;
	}

	t1 /= 1.0 * (i/25 + 1);
	t2 /= 1.0 * (i/25 + 1);

	printf("Average running time:\n");
	printf("    Algorithm 1: %.3f (s/frame)\n", t1/(double) CLOCKS_PER_SEC);
	printf("    Algorithm 2: %.3f (s/frame)\n", t2/(double) CLOCKS_PER_SEC);

	cap.release();
	destroyAllWindows();
	return 1;
}

int main(){
	readVideo(DEFAULT_VIDEO.c_str());
	return 0;
}



Mat		tre0(Mat src) {
	//find textbox
		//gray = gray(Rect(60, 380, 530, 70));
		//gray = findTextbox(gray);
		//resize(gray, gray, Size(2 * gray.cols, 2 * gray.rows));

		//Edge drawing
		//edge1 = ED::findEdge(gray, 56, 3, 2);

		//Canny
		//GaussianBlur(gray, gray, Size(7,7), 1.5, 1.5);
		//Canny(gray, edge2, 30, 70, 3);
		
		//view frame
		//imshow("ED", edge1);
		//imshow("Canny", edge2);
	return Mat();
}

Mat		edgedetect(const Mat &src) {
	Mat edge, dilation, thresMat, gray;
	//Canny
	GaussianBlur(src, gray, Size(7,7), 1.5, 1.5);
	Canny(gray, edge, 30, 70, 3);
	//edge = ED::findEdge(gray, 24, 8, 4);
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(2,2));
	dilate(edge, dilation, element);
	return edge;
}