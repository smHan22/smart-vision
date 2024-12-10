# 라인트레이서_최종 실험

➨ 라인트레이서의 최종 결과물을 소개하겠습니다.

(코드 단위로의 설명은 저번 시간에 다뤘기 때문에, 현재 레포트에서는 흐름 위주로 설명하겠습니다.)

# < main.cpp >

https://github.com/smHan22/smart-vision/blob/858b1031f2dd84910c28cb426fa494621a94de90/line_tracer/linetracer_real/main.cpp#L1-L107

```
⦁ 역할 : 프로그램의 메인 로직을 담당하며, 전체적인 흐름을 담당

⦁ 처리 흐름

  1. 카메라 스트림 초기화 : jetson의 GStreamer 파이프라인을 통해 카메라 입력 스트림을 설정하고 확인

  2. GStreamer 출력 스트림 설정 : 원본 프레임과 이진화된 프레임을 네트워크로 전송할 준비를 함

  3. 장치 초기화 : Dynamixel 모터 제어 인터페이스 초기화, 오류 발생 시 프로그램 종료

  4. 프레임 처리

    - 카메라에서 프레임을 읽고, 이를 전처리(preprocessFrame) 및 라인 중심 에러 계산(calculateError)에 전달

    - 에러 값을 기반으로 모터 속도를 계산하고 모터를 제어

    - GStreamer 출력 스트림에 처리된 프레임을 전송

    - 일정한 프레임 속도를 유지하기 위해 타이밍 조정

  5. 종료 처리 : 스트림과 모터 연결을 종료 
```

# < vision.cpp >

https://github.com/smHan22/smart-vision/blob/a9e7833dae1de6421bd1a2274e8094e556dc4f7e/line_tracer/linetracer_real/vision.cpp#L1-L97

```
⦁ 역할 : 영상 데이터의 전처리와 라인 중심 게산 및 에러 측정을 처리

⦁ 처리 흐름

  1. preprocessFrame
    - 프레임을 그레이스케일로 변환

    - 밝기 보정을 통해 일정한 조명 조건 유지

    - 이진화 및 관심 영역 설정으로 하단 1/4 부분을 집중 분석

    - 이진화된 결과를 컬러 이미지로 변환하여 시각적으로 표시

  2. calculateError

    - 연결된 구성 요소를 분석하여 라인의 중심점들을 추출

    - 첫 번째 프레임에서는 이미지 중심과 가장 가까운 라인 중심을 선택

    - 이후 프레임에서는 이전 중심점과 가장 가까운 중심점을 선택

    - 중심점 간 에러를 계산하고, 에러에 따라 라인의 위치를 보정
```

# < vision.hpp >

https://github.com/smHan22/smart-vision/blob/47c4404289fae067b3a2ccfa4127a90157449ffb/line_tracer/linetracer_real/vision.hpp#L1-L16

```
⦁ 역할 : vision.cpp에서 정의된 전처리 및 에러 계산 함수를 외부에서 사용할 수 있도록 제공

⦁ 구조

    - preprocessFrame : 프레임 처리

    - calculateError : 중심점 및 에러 계산
```

# < dxl.cpp >

https://github.com/smHan22/smart-vision/blob/f0f8632b0f4e3c64ed70bce70dd83025d3629a14/line_tracer/linetracer_real/dxl.cpp#L1-L606

```
⦁ 주요 기능 설명

  1. getch 함수

    - 사용자가 키 입력을 받는 함수입니다. 터미널에서 비차단 방식으로 키 입력을 감지합니다.

  2. kbhit 함수

    - getch 함수와 유사하지만, 입력이 없으면 바로 false를 반환하고, 입력이 있으면 true를 반환합니다.

  3. open 함수

    - 포트를 열고, 보드 레이트를 설정한 뒤, Dynamixel 모터에 토크를 활성화하여 연결을 설정합니다. 이때, 두 개의 모터에 대해 토크를 활성화합니다.

  4. close 함수

    - 모터의 속도를 0으로 설정하여 멈추고, 연결된 Dynamixel 모터의 토크를 비활성화한 후 포트를 닫습니다.

  5. setVelocity 함수

    - 목표 RPM 값을 받아서 해당 속도로 두 개의 Dynamixel 모터를 동기화하여 제어합니다.

    - 각 모터에 대해 목표 속도를 설정하고, 이를 GroupSyncWrite를 사용하여 동시에 전송합니다.

  6. velConvert 함수

    - RPM 값을 Dynamixel이 사용하는 속도 값으로 변환합니다.
```

# < dxl.hpp >

https://github.com/smHan22/smart-vision/blob/42d46b25b047abd7ca1e065197c9d325657db31c/line_tracer/linetracer_real/dxl.hpp#L1-L102

```
⦁ 헤더 파일 보호 메크로 ( #ifndef, #define, #endif )

  - 동일 파일이 여러 번 포함되는 것을 방지합니다.

⦁ 모델 정의 ( MX12W, XC430W150, XL430W250 )

  - Dynamixel 모터의 다양한 모델을 선택할 수 있도록 설정합니다.

⦁ 제어 테이블 주소

  - 각 모델의 다양한 설정에 필요한 메모리 주소들을 정의합니다.

⦁ Dxl 클래스

  - port_num, group_num 등의 멤버 변수를 통해 포트 설정과 그룹 설정을 다룹니다.

  - open()과 close() 메서드를 통해 포트를 열고 닫는 기능을 제공합니다.

  - setVelocity()는 목표 RPM 값을 설정하고, velConvert()는 RPM을 속도 값으로 변환합니다.

  - getch()와 kbhit()은 키보드 입력을 다루는 함수입니다.
```

# < Makefile >

https://github.com/smHan22/smart-vision/blob/8a73953021dff9e5aa956da2ed8fbc2ad2e36d75/line_tracer/linetracer_real/Makefile#L1-L23

```
⦁
```
