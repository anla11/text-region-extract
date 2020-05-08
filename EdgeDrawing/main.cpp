#include "../MyLib/EdgeDrawingAlg.h"
#include <time.h>

Mat		useCanny(const Mat &gray){
	Mat edge;
	Canny(gray, edge, 30, 50, 3);
	return edge;	
}

int main(){
	string path = string(DEFAULT_INPUT) + string("Chairs.bmp");
	Mat img = imread(path.c_str());
	imshow("image", img);

	Mat gray, edge;
	clock_t start, end;

	cvtColor(img, gray, CV_BGR2GRAY);
	//GaussianBlur(gray, gray, Size(5,5), 0);
	blur(gray, gray, Size(5, 5));

	cout << "Edge Detection Algorithms\n";
	
	start = clock();
	edge = ED::findEdge(gray, 20, 8, 4);
	end = clock();
	imshow("Edge Drawing", edge);
	cout << "Edge Drawing running time: " << (end-start) /  (double) CLOCKS_PER_SEC << " s\n";
	
	start = clock();
	
	edge = useCanny(gray);  
	end = clock();
	imshow("Canny", edge);
	cout << "Canny of OpenCV Library running time: " << (end-start) /  (double) CLOCKS_PER_SEC << " s\n";
	
	waitKey(0);
	return 0;
}	