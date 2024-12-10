#include "opencv2/opencv.hpp"
#include <iostream>
#include <unistd.h>
#include "dxl.hpp"
#include <sys/time.h>
#include "vision.hpp"

using namespace cv;
using namespace std;

bool ctrl_c_pressed = false;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main() {
     string videoPath = "nvarguscamerasrc sensor-id=0 ! \
        video/x-raw(memory:NVMM), width=(int)640, height=(int)360, \
        format=(string)NV12, framerate=(fraction)30/1 ! \
        nvvidconv flip-method=0 ! video/x-raw, \
        width=(int)640, height=(int)360, format=(string)BGRx ! \
        videoconvert ! video/x-raw, format=(string)BGR ! appsink";

    VideoCapture cap(videoPath, CAP_GSTREAMER);

    if (!cap.isOpened()) {
        cerr << "Error: Unable to open video file!" << endl;
        return -1;
    }

    string dst1 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
    nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! h264parse ! \
    rtph264pay pt=96 ! udpsink host=203.234.58.163 port=8001 sync=false";
    string dst2 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
    nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! h264parse ! \
    rtph264pay pt=96 ! udpsink host=203.234.58.163 port=8002 sync=false";

    VideoWriter writer1(dst1, 0, 30, Size(640, 360), true);
    VideoWriter writer2(dst2, 0, 30, Size(640, 90), true);

    if (!writer1.isOpened() || !writer2.isOpened()) {
        cerr << "Error: Unable to open GStreamer writers!" << endl;
        return -1;
    }

    Dxl dxl;
    if (!dxl.open()) { 
    cout << "dxl open error" << endl; 
    return -1; 
    }

    Mat frame, binary, colorBinary;
    Point previousCenter(-1, -1);  // 이전 중심점
    double error;           // 초기 error
    bool firstFrame = true;       // 첫 번째 프레임 여부
    bool mode = false;
    const int targetDelayMs = 30; // 목표 프레임 지연(ms)
    const double MAX_DISTANCE = 100.0; // 라인 후보 간 최대 거리
    double k = 0.2;               // 보정 계수

    struct timeval start, end1;

    while (true) {
        gettimeofday(&start, NULL);
        cap >> frame;

        if (frame.empty()) {
            cerr << "Error: Empty frame received!" << endl;
            break;
        }

        preprocessFrame(frame, binary, colorBinary);

        error = calculateError(binary, colorBinary, previousCenter, firstFrame, MAX_DISTANCE, error);

        if(dxl.kbhit())
        {
            char ch = dxl.getch();
            if(ch == 'q') break;
            else if(ch == 's') {mode = true; cout << "moter on!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;}
        }

        if(ctrl_c_pressed) break;
        double leftVel = 200 - k* error;
        double rightVel = -(200 + k* error);
        if(mode) dxl.setVelocity(leftVel, rightVel);

        writer1 << frame;
        writer2 << colorBinary;

        // 프레임 타이밍 조정
        gettimeofday(&end1, NULL);
        double elapsedMs = (end1.tv_sec - start.tv_sec) * 1000.0 + (end1.tv_usec - start.tv_usec) / 1000.0;

        int sleepMs = targetDelayMs - static_cast<int>(elapsedMs);
        if (sleepMs > 0) {
            usleep(sleepMs * 1000);
        }
        double totalTime = elapsedMs + sleepMs;

        cout << "error: " << error << ", leftVel: " << leftVel << ", rightVel: " << rightVel << ", time: " << totalTime << "ms" << endl;
    }
    cap.release();
    dxl.close();
    return 0;
}
