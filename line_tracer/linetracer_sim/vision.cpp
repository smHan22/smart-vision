#include "vision.hpp"

void preprocessFrame(const Mat& frame, Mat& binary, Mat& colorBinary) {
    Mat gray;

    // 그레이스케일 변환
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    // 밝기 보정
    Scalar meanBrightness = mean(gray);
    gray = gray + (100 - meanBrightness[0]);

    // 이진화
    threshold(gray, binary, 130, 255, THRESH_BINARY);

    // ROI 설정 (하단 4/1 부분)
    Rect roi(0, 270, 640, 90);
    binary = binary(roi);

    // 컬러 변환
    cvtColor(binary, colorBinary, COLOR_GRAY2BGR);
}

double calculateError(const Mat& binary, Mat& colorBinary, Point& previousCenter, bool& firstFrame, double maxDistance, double error) {
    Mat labelImage, stats, centroids;

    int nLabels = connectedComponentsWithStats(binary, labelImage, stats, centroids, 8, CV_32S);

    vector<Point> lineCenters;      // 라인의 중심점을 저장할 벡터
    for (int i = 1; i < nLabels; i++) {
        Point center = Point(centroids.at<double>(i, 0), centroids.at<double>(i, 1));       // 중심점은 centroids에서 가져옴
        lineCenters.push_back(center);      // 중심점을 벡터에 추가

        // 바운딩 박스의 위치와 크기 계산 (stats에서 가져옴)
        int x = stats.at<int>(i, CC_STAT_LEFT);   // 바운딩 박스의 왼쪽 상단 x 좌표
        int y = stats.at<int>(i, CC_STAT_TOP);    // 바운딩 박스의 왼쪽 상단 y 좌표
        int width = stats.at<int>(i, CC_STAT_WIDTH);  // 바운딩 박스의 너비
        int height = stats.at<int>(i, CC_STAT_HEIGHT); // 바운딩 박스의 높이

        // 바운딩 박스 그리기: 실제 객체 크기에 맞춰 그리기
        rectangle(colorBinary, Rect(x, y, width, height), Scalar(255, 0, 0), 2);
        circle(colorBinary, center, 5, Scalar(255, 0, 0), -1);
    }

    if (firstFrame && !lineCenters.empty()) {
        // 첫 프레임에서 가장 가까운 중심점 찾기
        Point centerOfImage(binary.cols / 2, binary.rows / 2);
        double minDistance = DBL_MAX;
        Point closestCenter;

        for (size_t i = 0; i < lineCenters.size(); i++) {
            double distance = norm(lineCenters[i] - centerOfImage);
            if (distance < minDistance) {
                minDistance = distance;
                closestCenter = lineCenters[i];
            }
        }

        // 바운딩 박스 그리기
        rectangle(colorBinary, Rect(closestCenter.x - 10, closestCenter.y - 10, 20, 20), Scalar(0, 0, 255), 2);
        circle(colorBinary, closestCenter, 5, Scalar(0, 0, 255), -1);

        error = centerOfImage.x - closestCenter.x;

        previousCenter = closestCenter;
        firstFrame = false;

        return error;
    }

    else if (previousCenter.x != -1 && !lineCenters.empty()) {
        // 이전 중심점 기준으로 가장 가까운 중심점 찾기
        double minDistance = DBL_MAX;
        Point closestCenter;

        for (size_t i = 0; i < lineCenters.size(); i++) {
            double distance = norm(lineCenters[i] - previousCenter);
            if (distance < minDistance && distance < maxDistance) {
                minDistance = distance;
                closestCenter = lineCenters[i];
            }
        }

        if (minDistance < maxDistance) {
            rectangle(colorBinary, Rect(closestCenter.x - 10, closestCenter.y - 10, 20, 20), Scalar(0, 0, 255), 2);
            circle(colorBinary, closestCenter, 5, Scalar(0, 0, 255), -1);
            previousCenter = closestCenter;
        } else {        // 이전 중심점 그리기
            rectangle(colorBinary, Rect(previousCenter.x - 10, previousCenter.y - 10, 20, 20), Scalar(0, 0, 255), 2);
            circle(colorBinary, previousCenter, 5, Scalar(0, 0, 255), -1);
        }
        Point centerOfImage(binary.cols / 2, binary.rows / 2);
        error = centerOfImage.x - closestCenter.x;

        return error;
    }
}
