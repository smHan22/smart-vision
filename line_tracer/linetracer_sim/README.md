# 라인트레이서_모션제어

➨ 지난 시간 라인 검출에 이어서 검출한 라인과 위치오차를 이용하여 속도명령을 설계하는 코드를 추가하였습니다.
(지난 시간 main.cpp 코드를 main.cpp, vision.cpp, vision.hpp로 분할 컴파일 작성하였습니다.)

# main.cpp

https://github.com/smHan22/smart-vision/blob/ad8bba96dd0172ba8d949c0e0e9f5791d2fb3ff2/line_tracer/linetracer_sim/main.cpp#L12-L17

```ruby
⦁ ctrl_c_pressed: Ctrl+c가 눌렸는지 확인하는 변수

⦁ ctrlc: 시그널 처리 함수로, Ctrl+c가 눌리면 ctrl_c_pressed를 true로 설정
```

https://github.com/smHan22/smart-vision/blob/b9d233199fa51126f15032b0a4ecd5edeaf8e79a/line_tracer/linetracer_sim/main.cpp#L42-L46

```ruby
⦁ DXL 모터를 제어하기 위해 DXL 객체를 생성하고, dxl.open()을 통해 모터가 열리지 않으면 오류 메시지를 출력하고 종료
```

https://github.com/smHan22/smart-vision/blob/5b33bc9dc7cc3d043d072c3cf14f4ede4e337a1b/line_tracer/linetracer_sim/main.cpp#L48-L55

```ruby
⦁ frame: 현재 비디오 프레임을 저장할 Mat 객체

⦁ binary, colorBinary: 이진화된 이미지와 컬러 이진화 이미지

⦁ previousCenter: 이전에 계산된 중심점

⦁ error: 라인 추적에서 발생하는 에러 값

⦁ firstFrame: 첫 번째 프레임인지 확인하는 변수

⦁ mode: 모터 작동 상태

⦁ targetDelayMs: 목표 프레임 지연 시간

⦁ MAX_DESTANCE: 라인 후보 간의 최대 거리

⦁ k: 보정 계수 (게인 값)
```

https://github.com/smHan22/smart-vision/blob/52e3232308865dd5351182656d5879251bb6bd85/line_tracer/linetracer_sim/main.cpp#L69-L72

```ruby
⦁ 비디오에서 프레임을 읽고, preprocessFrame함수를 사용하여 전처리 및 이진화를 수행

⦁ calculateError 함수로 라인의 중심을 계산하고 에러 값을 업데이트
```

https://github.com/smHan22/smart-vision/blob/ee9d902bbc08834a76789637743c26d6b045122a/line_tracer/linetracer_sim/main.cpp#L75-L87

```ruby
⦁ 라인의 중심점을 기준으로 왼쪽 및 오른쪽 모터의 속도를 계산

⦁ dxl.kbhit()으로 키 입력을 체크하여 'q'를 입력하면 종료하고, 's'를 입력하면 모터를 작동시킴

⦁ ctrl_c_pressed가 ture이면 프로그램을 종료

⦁ dxl.setVelocity(leftvel, rightvel)은 계산된 속도를 기반으로 모터를 제어하여 방향 및 이동 속도를 설정.
```

https://github.com/smHan22/smart-vision/blob/9bb1e22e26532ce5f4e24f7536c6b83a504f5fcc/line_tracer/linetracer_sim/main.cpp#L89-L103

```ruby
⦁ elapsedMs: 처리된 시간 (프레임 처리 시간)을 밀리초 단위로 계산.

⦁ (end1.tv_sec - start.tv_sec) * 1000.0: 초 단위로 측정된 시간을 밀리초로 변환.
tv_sec는 초를 나타내며 이를 밀리초로 변환하려면 1000을 곱함

⦁ (end1.tv_usec - start.tv_usec) / 1000.0: 마이크로초 단위이므로 이를 1000으로 나누어 밀리초로 변환

⦁ sleepMs: 목표 지연 시간에서 실제 처리 시간을 빼서, 추가로 기다려야 할 시간을 계산
static_cast<int>(dlapsedMs)는 elapsedMs를 정수형으로 변환하여 사용

⦁ usleep(sleepMs * 1000): sleepMs는 밀리초 단위이므로, 이를 마이크로초 단위로 변환하려면 1000을 곱함.
usleep 함수는 마이크로초 단위로 대기하기 때문에 이 값을 인자로 전달.

⦁ totalTime: 실제 처리 시간과 대기 시간을 더한 총 소요 시간을 계산.
```
# vision.cpp

https://github.com/smHan22/smart-vision/blob/0284e87ec10ae1f4fcf2e07bcbf64baf8db1f9db/line_tracer/linetracer_sim/vision.cpp#L3

```ruby
⦁ 비디오 프레임을 전처리하는 함수 (그레이스케일 변환, 밝기 보정, 크기 설정, 이진화 등)
```

https://github.com/smHan22/smart-vision/blob/947786a83a5b8034b814f7905264827f28c610ea/line_tracer/linetracer_sim/vision.cpp#L24-L25

```ruby
⦁ 이진화된 이미지에서 연결된 객체를 찾고, 라인의 중심을 계산하여 오류 값을 반환하는 함수
```

https://github.com/smHan22/smart-vision/blob/0e99677ebac2ff846c6e7ad6dd4c12d6e3b17619/line_tracer/linetracer_sim/vision.cpp#L25-L27

```ruby
⦁ labelImage: 각 픽셀의 레이블

⦁ stats: 각 연결 컴포넌트의 바운딩 박스 정보

⦁ centroids: 각 컴포넌트의 중심점
```

https://github.com/smHan22/smart-vision/blob/1ea1e7208bd5d5d5ebab56b3cbb055cea4b48ba0/line_tracer/linetracer_sim/vision.cpp#L29-L43

```ruby
⦁ vector<Point> lineCenters: 각 영역의 중심점을 저장할 벡터

⦁ center: 현재 영역의 중심점을 계산
centriuds는 각 레이블의 중심 좌표를 담고 있는 행렬
centroids.at<double>(i, 0> : i번째 레이블의 x좌표
centroids.at<double>(i, 1> : i번째 레이블의 y좌표

⦁ lineCenters.push_back(center) : 계산한 중심점을 lineCenters 백터에 추가

⦁ 영역에 바운딩 박스와 바운딩 박스의 중심점에 파란색으로 원을 그림

```

https://github.com/smHan22/smart-vision/blob/02d4efdcbd69729fc990d64b8b383760c737445f/line_tracer/linetracer_sim/vision.cpp#L45-L69

```ruby
⦁ if (firstFrame && !lineCenters.empty()): 현재 첫 번째 프레임인지 확인, 탐지한 라인의 중심점이 하나 이상 존재하는지

⦁ Point centerOfImage(binary.cols / 2, binary.rows / 2) : 이미지의 정중앙을 계산

⦁ minDistance : 표현 가능한 가장 큰 실수 값으로 초기화하여 발견되는 거리 값이 항상 초기값보다 작아지도록 설정

⦁ closestCenter : 가장 가까운 중심점을 저장할 변수

⦁ for (size_t i = 0; i < lineCenters.size(); i++) : 탐지된 모든 라인의 중심점을 순회

⦁ distance : 두 점 간의 유클리드 거리를 계산

⦁ if(distance < minDistance) : 현재 계산된 거리가 최소 거리보다 작은지 확인.

⦁ 가장 가까운 중심점 주변에 빨간색 바운딩 박스와 원을 그림

⦁ error = centerOfImage.x - closestCenter.x : 에러 값 계산

⦁ 가장 가까운 중심점을 previousCenter에 저장하고, firstFrame을 flase로 설정하여 이후에는 이 조건문을 실행하지 않도록 설정
```

https://github.com/smHan22/smart-vision/blob/a103774308910c37769c17daa736f77dc6747261/line_tracer/linetracer_sim/vision.cpp#L71-L97

```ruby
⦁ else if(previousCenter.x != -1 && !lineCenters.empty()) : 이전 중심점이 있고, 현재 프레임에서
탐지된 중심점이 하나 이상 존재하는지 확인

⦁ distance : 현재 중심점과 이전 중심점간의 거리

⦁ if (distance < minDistance && distance < maxDistance) : 현재 중심점이 가장 가까운 거리인지,
최대 거리 안에 있는지 확인

⦁ if(minDistance < maxDistance) : 유효 거리 안에서 중심점을 찾았을 경우, 빨간색 바운딩 박스와 원을 그림
previousCenter를 업데이트

⦁ else : 유효한 중심점이 발견되지 않을 경우 (라인이 화면에서 사라진 경우)
이전 중심점을 빨간색 바운딩 박스와 원으로 표시
```
# vision.hpp

https://github.com/smHan22/smart-vision/blob/0ed9f403415e5337f095b250f3bc26ebcc0616c3/line_tracer/linetracer_sim/vision.hpp#L1-L16

```ruby
⦁ #ifndef VISION_HPP와 #define VISION_HPP : 헤더 파일이 중복으로 포함되는 것을 방지

⦁ #include "opencv2/opencv.hpp"와 #include <vector> : opencv 라이브러리와 STL의 vector를 포함

⦁ void preprocessFrame(const Mat& frame, Mat& binary, Mat& colorBinary) : 함수 선언
const Mat& frame : 입력 프레임
Mat& binary : 출력 이진화 영상
Mat& colorBinary : 출력 컬러 이진화 영상

⦁ double calculateError(const Mat& binary, Mat& colorBinary, Point& previousCenter, bool& firstFrame, double maxDistance, double error) : 함수 선언
const May& binary : 입력 이진화 영상
Mat& colorBinary : 출력 컬러 이진화 영상
Point& previousCenter : 이전 프레임에서의 중심점 좌표
bool& firstFrame : 첫 프레임인지 여부를 확인하는 플래그
double maxDistance : 중심점 간 유효 거리
double error : 이미지 중심점과 탐지된 중심점 간 x축 방향의 거리 오차
```


# Makefile

https://github.com/smHan22/smart-vision/blob/3c0a215444c069e2bf0cbc06ca37f6e3d3468c40/line_tracer/linetracer_sim/Makefile#L1-L7

```ruby
⦁ CX : 사용할 컴파일러를 지정

⦁ CXFLAGS : 컴파일 시 사용할 플래그 옵션

⦁ DXLFLAGS : Dynamixel SDK 관련 플래그

⦁ CVFLAGS : OpenCV 라이브러리를 사용하기 위한 플래그

⦁ TARGET : 빌드된 최종 실행 파일의 이름

⦁ OBJS : 빌드에 필요한 오브젝트 파일 목록
```

https://github.com/smHan22/smart-vision/blob/de1e00b2ea3e97cdb3392d546fb1bc6ceb978559/line_tracer/linetracer_sim/Makefile#L9-L10

```ruby
⦁ TARGET을 생성 (main.o, dxl.o, vision.o를 링크하여 moter_sim이라는 실행 파일 생성)
```

https://github.com/smHan22/smart-vision/blob/7f1731ade56bddf624dc772a71dd59ab12aa7fef/line_tracer/linetracer_sim/Makefile#L12-L19

```ruby
⦁ main.o : main.cpp를 컴파일하여 오브젝트 파일 생성

⦁ dxl.o : dxl.cpp를 컴파일하여 dxl.o 생성

⦁ vision.o : vision.cpp를 컴파일하여 vision.o 생성
```

https://github.com/smHan22/smart-vision/blob/ea3010f4b34b136657083e8bc2b7444bb2043010/line_tracer/linetracer_sim/Makefile#L21-L23

```ruby
⦁ clean은 실제 파일 이름이 아니므로, 이를 유틸리티 타겟으로 설정

⦁ 빌드된 실행 파일 삭제하는 명령어
```

# 실행영상

https://youtu.be/bKOA_ZvVlF0
