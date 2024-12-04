#ifndef VISION_HPP
#define VISION_HPP

#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;
using namespace std;

// 전처리 함수
void preprocessFrame(const Mat& frame, Mat& binary, Mat& colorBinary);

// 라인 중심 계산 및 에러 반환 함수
double calculateError(const Mat& binary, Mat& colorBinary, Point& previousCenter, bool& firstFrame, double maxDistance, double error);

#endif // VISION_HPP
