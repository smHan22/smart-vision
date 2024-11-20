#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>
#include <sys/time.h>
using namespace cv;
using namespace std;

int main() {
    string videoPath = "/home/hsmin22/workspace/line_tracer/simulation/5_lt_cw_100rpm_out.mp4";

    VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        cerr << "Error: Unable to open video file!" << endl;
        return -1;
    }

    double fps = cap.get(CAP_PROP_FPS);
    int delay = cvRound(1000 / fps);

    Mat frame, gray, binary, resizedBinary;
    Point previousCenter(-1, -1);  // 이전 중심점을 저장할 변수
    bool firstFrame = true;  // 첫 번째 프레임을 처리하는지 여부
    const double MAX_DISTANCE = 100.0;  // 라인 후보 간의 최대 허용 거리
    struct timeval start, end1;
    double diff1;
    double error = 0.0;  // error 값을 처음에 0으로 초기화
    while (true) {
        gettimeofday(&start, NULL);

        cap >> frame;
        if (frame.empty()) {
            cerr << "Error: Empty frame received!" << endl;
            break;
        }

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        threshold(gray, binary, 130, 255, THRESH_BINARY);

        Rect roi(0, 270, 640, 90);
        resizedBinary = binary(roi);        // resizedBinary는 4/1로 자른 영상 이진화 시킨 결과 영상

        Mat labelImage, stats, centroids;
        int nLabels = connectedComponentsWithStats(resizedBinary, labelImage, stats, centroids, 8, CV_32S);

        Mat colorBinary;
        cvtColor(resizedBinary, colorBinary, COLOR_GRAY2BGR);       // 컬러 영상으로 변환

        vector<Point> lineCenters;  // 라인의 중심점을 저장할 벡터
        for (int i = 1; i < nLabels; i++) {  // 0번 라벨은 배경이므로 1번부터 시작
            int x = stats.at<int>(i, CC_STAT_LEFT);   // 바운딩 박스의 왼쪽 상단 x 좌표
            int y = stats.at<int>(i, CC_STAT_TOP);    // 바운딩 박스의 왼쪽 상단 y 좌표
            int width = stats.at<int>(i, CC_STAT_WIDTH);  // 바운딩 박스의 너비
            int height = stats.at<int>(i, CC_STAT_HEIGHT); // 바운딩 박스의 높이
            int area = stats.at<int>(i, CC_STAT_AREA);  // 객체의 넓이

            // 각 객체의 중심점 계산
            Point center = Point(centroids.at<double>(i, 0), centroids.at<double>(i, 1));  // 중심점은 centroids에서 가져옴
            lineCenters.push_back(center);  // 중심점을 벡터에 추가

            // 객체의 바운딩 박스를 그리기
            rectangle(colorBinary, Rect(x, y, width, height), Scalar(255, 0, 0), 2);  // 바운딩 박스는 빨간색 (0, 0, 255)
            circle(colorBinary, center, 5, Scalar(255, 0, 0), -1);  // 중심점은 빨간색 원으로 그리기
        }

        if (firstFrame && !lineCenters.empty()) {       // firstFrame이 true인 경우와 객체 중심점이 하나 이상 있을 때 조건
            Point centerOfImage(frame.cols / 2, frame.rows / 2);     // 이미지의 중앙 좌표를 계산
            double minDistance = DBL_MAX;       // 중앙 좌표와 각 객체의 중심점 간의 최소 거리를 저장할 변수 (DBL_MAX는 double 타입이 가질 수 있는 최대값)
            Point closestCenter;        // 각 객체의 중심점 간의 최소 거리를 저장할 변수

            for (int i = 0; i < lineCenters.size(); i++) {      // 백터에 저장된 각 라인의 중심점을 순차적으로 확인
                double distance = norm(lineCenters[i] - centerOfImage);     // 객체의 중심점과 이미지의 중앙점의 거리를 계산 (norm은 두 점 사이의 유클리드 거리를 계산)

                if (distance < minDistance) {       // 계산된 distance가 minDistance보다 작으면
                    minDistance = distance;     // 값 저장
                    closestCenter = lineCenters[i];
                }
            }
            // 찾은 중심점에 바운딩 박스와 점 그리기
            rectangle(colorBinary, Rect(closestCenter.x - 10, closestCenter.y - 10, 20, 20), Scalar(0, 0, 255), 2);
            circle(colorBinary, closestCenter, 5, Scalar(0, 0, 255), -1);

            error = centerOfImage.x - closestCenter.x;  // 영상의 중심과 라인의 중심 x좌표 차이

            previousCenter = closestCenter;     // 현재 찾은 가장 가까운 중심점을 저장하여, 다음 프레임에서 이 중심점을 기준으로 라인을 추적할 수 있도록 함
            firstFrame = false;
        }

        else if (previousCenter.x != -1 && !lineCenters.empty()) {      // 중심점을 찾은 경우와 객체 중심점이 하나 이상 있을 때의 조건
            double minDistance = DBL_MAX;       // 중앙 좌표와 각 객체의 중심점 간의 최소 거리를 저장할 변수 (DBL_MAX는 double 타입이 가질 수 있는 최대값)
            Point closestCenter;        // 가장 가까운 중심점을 저장할 변수
            for (int i = 0; i < lineCenters.size(); i++) {      // 백터에 저장된 각 라인의 중심점을 순차적으로 확인
                double distance = norm(lineCenters[i] - previousCenter);        // 객체의 중심점과 이전 중심점간의 거리를 계산 norm(두 점의 차이를 제곱한 후 제곱근을 취해 실제 거리를 반환)
                if (distance < minDistance && distance < MAX_DISTANCE) {        // 계산된 거리가 모두 작으면
                    minDistance = distance;     // 값 저장
                    closestCenter = lineCenters[i];     // 가장 가까운 중심점이 발견되면, 라인의 중심점을 closestCenter에 저장
                }
            }

            if (minDistance < MAX_DISTANCE) {       // 가장 가까운 라인 중심점이 이전 중심점과 충분히 가까운 경우
                rectangle(colorBinary, Rect(closestCenter.x - 10, closestCenter.y - 10, 20, 20), Scalar(0, 0, 255), 2);     // 파란색 바운딩 박스
                circle(colorBinary, closestCenter, 5, Scalar(0, 0, 255), -1);       // 원 그림
                previousCenter = closestCenter;     // 현재 라인 중심점을 이전 중심점에 저장
            } 
            else {      // minDistance가 <MAX_DISTANCE보다 크면 (라인이 사라진 경우 또는 너무 멀리 떨어진 경우)
                rectangle(colorBinary, Rect(previousCenter.x - 10, previousCenter.y - 10, 20, 20), Scalar(255, 0, 0), 2);       // 빨간색 바운딩 박스
                circle(colorBinary, previousCenter, 5, Scalar(255, 0, 0), -1);      // 원 그림
            }

            // error 값도 갱신
            Point centerOfImage(frame.cols / 2, frame.rows / 2);     // 이미지의 중앙 좌표를 계산
            error = centerOfImage.x - closestCenter.x;  // 영상의 중심과 라인의 중심 x좌표 차이
        }

        imshow("original", frame);
        imshow("bounding box", colorBinary);

        if (waitKey(delay) == 27) {
            break;
        }

        gettimeofday(&end1, NULL);
        diff1 = end1.tv_sec + end1.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0;
        cout << "error: " << error << ", " << "time: " << diff1 << endl;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
