#include "EdgeDrawingAlg.h"

namespace ED {
	const int VERTICAL = 1;
	const int HORIZONTAL = 0;	
	const int POSITIVE = 1;
	const int NEGATIVE = -1;
	const double BOTH_EDGE = 0;

	typedef pair<int, int> pii;
	typedef vector< vector<int> > vvi;

	// Hàm phụ
	int		checkPixel(const pii &pos, const Size &size) {
		int x = pos.first, y = pos.second;
		return (x >= 0 && y >= 0 && x < size.height && y < size.width);
	}

	// Hàm xử lý grmapient và drmap
	void	calGrDr(const Mat &image, Mat &grmap, Mat &drmap, double thres) {
		Mat grx, gry, bin;
		double Min, Max;

		Sobel(image, grx, CV_32F, 1, 0, 5);
		Sobel(image, gry, CV_32F, 0, 1, 5);
		grx = abs(grx);
		gry = abs(gry);
		drmap = grx - gry;

		grmap = grx + gry;
		//magnitude(grx, gry, grmap);

		minMaxLoc(grmap, &Min, &Max);
		threshold(grmap, bin, Max/256*thres, 1, THRESH_BINARY);
		multiply(grmap, bin, grmap);
	}

	pii 	getDirect(double gr, double dr){
		if (gr == 0) return make_pair(0, 0);
		if (abs(dr) <= BOTH_EDGE) return make_pair(1, 0);
		if (dr > 0)  return make_pair(1, 1);
		return make_pair(1, -1);
	}

	int		isHorizontal(double gr, double dr){
		pii edge = getDirect(gr, dr);
		return (edge.first == 1) && (edge.second != 1);
	}

	int		isVertical(double gr, double dr){
		pii edge = getDirect(gr, dr);
		return (edge.first == 1) && (edge.second != -1);
	}

	// Hàm xử lý anchor

	int		isAnchor(const pii &pos, const Mat &grmap, const Mat &drmap, const double &thres) {
		int x = pos.first, y = pos.second;
		if (isVertical(pxf(grmap, x, y), pxf(drmap, x, y)) &&
			(pxf(grmap, x, y) - pxf(grmap, x, y-1) >= thres) && (pxf(grmap, x, y) - pxf(grmap, x, y+1) >= thres))
				return 1;
		if (isHorizontal(pxf(grmap, x, y), pxf(drmap, x, y)) &&
			(pxf(grmap, x, y) - pxf(grmap, x-1, y) >= thres) && (pxf(grmap, x, y) - pxf(grmap, x+1, y) >= thres))
				return 1;
		return 0;
	}

	vector<pii>		findAnchor(const Mat &grmap, const Mat &drmap, const double &thres, const double &step) {
		vector<pii> res;
		res.resize(0);
		for (int i = 1, height = grmap.rows; i < height-1; i += step) {
			for (int j = 1, width = grmap.cols; j < width-1; j += step) {
				pii pos = make_pair(i, j);
				if (isAnchor(pos, grmap, drmap, thres))
					res.push_back(pos);
			}
		}
		return res;
	}

	// Nối cạnh
	pii		go(pii pos, Mat &edge, const Mat &grmap, const Mat &drmap, pii direct) {
		int x = pos.first, y = pos.second;

		// kiểm tra đổi hướng
		if ((direct.first == VERTICAL && !isVertical(pxf(grmap, x, y), pxf(drmap, x, y))) ||
			(direct.first == HORIZONTAL && !isHorizontal(pxf(grmap, x, y), pxf(drmap, x, y))))
				return pos;

		while (1) {
			// tìm pos mới từ x, y
			pos = make_pair(-1, -1);
			double Max = 0;
			for (int i = -1; i <= 1; ++i) {
				pii tmp;
				if (direct.first == VERTICAL)
					tmp = make_pair(x + direct.second, y + i);
				else
					tmp = make_pair(x + i, y + direct.second);
				if (checkPixel(tmp, grmap.size()) && pxf(grmap, tmp.first, tmp.second) > Max) {
					Max = pxf(grmap, tmp.first, tmp.second);
					pos = tmp;
				}
			}
			x = pos.first, y = pos.second;
			// kiểm tra điểm hợp lệ
			if (!checkPixel(pos, grmap.size()) || pxf(edge, x, y) == 1)/*(pxf(edge, x, y) >= 0.2 && pxf(edge,x , y) < 0.5))*/
				return make_pair(-1, -1);
			// kiểm tra đổi hướng
			if ((direct.first == VERTICAL && !isVertical(pxf(grmap, x, y), pxf(drmap, x, y))) ||
				(direct.first == HORIZONTAL && !isHorizontal(pxf(grmap, x, y), pxf(drmap, x, y))))
				return make_pair(x, y);
			pxf(edge, x, y) = 1;
		}
	}

	void	connect_edge(Mat &edge, const Mat &grmap, const Mat &drmap, vector<pii> &anchors) {
		int row = grmap.rows, col = grmap.cols;

		Mat ispush = Mat(edge.size(), CV_32FC1, Scalar_<float>(0));
		for (int i = 0, sz = anchors.size(); i < sz; ++i) 
			pxf(ispush, anchors[i].first, anchors[i].second) = 1;

		while (!anchors.empty()) {
			pii pos = anchors[anchors.size() - 1];
			anchors.pop_back();
			if (pxf(edge, pos.first, pos.second) != 1) {
				for (int dr = 0; dr <= 1; ++dr) {
					for (int tdc = -1; tdc <= 1; tdc += 2) {
						pii next = go(pos, edge, grmap, drmap, make_pair(dr,tdc));
						if (next.first != -1 && next != pos && !pxf(ispush,next.first,next.second)) {
							anchors.push_back(next);
							pxf(ispush,next.first,next.second) = 1;
						}
					}
				}
				pxf(edge, pos.first, pos.second) = 1;
			}

			//imshow("connect edge", edge);
			//waitKey(1);
		}
	}

	Mat		findEdge(const Mat &gray, double thres1, double thres2, int step) {
		Mat grmap, drmap;
		calGrDr(gray, grmap, drmap, thres1);
		vector<pii> anchors = findAnchor(grmap, drmap, thres2, step);

		Mat edge = Mat(grmap.size(), CV_32FC1, Scalar_<float>(0));
		connect_edge(edge, grmap, drmap, anchors);
		return edge;
	}
}
