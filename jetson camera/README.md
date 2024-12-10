# Jetson Nano에서 카메라 영상 실시간 전송 및 변환

➨ Jetson Nano를 이용하여 카메라 영상을 실시간으로 수신하고, 해당 영상을 세 가지 형태(원본, 그레이스케일, 이진화)로 전송하는 시스템 구현

• 각각의 영상은 특정 포트를 통해 네트워크로 전송되며, OpenCV와 GStreamer를 사용하여 Jetson Nano의 영상을 처리

```
• 개발 환경 : Jetson Nano
  
• 라이브러리 OpenCV (GStreamer 지원 포함)
  
• 네트워크 송출 포드:
  
          원본 영상: 포트 8001
  
          그레이스케일 영상: 포트 8002
  
          이진화 영상: 포트 8003
```


# Makefile

• Makefile은 프로젝트의 빌드 과정을 자동화하는 파일로, 여러 파일이 포함된 프로젝트를 쉽게 컴파일하고 관리할 수 있도록 함.

• 빌드 규칙은 터미널에서 make 명령어를 입력하면 $(TARGET):$(STCS) 규칙이 실행되어 camera 실행 파일을 생성.

https://github.com/smHan22/smart-vision/blob/e58f2652ed96c215bd7fa4cb8552c2bb6410d163/jetson%20camera/Makefile#L1-L11

```
CX: 컴파일러를 지정
  
CVFLAGS: OpenCV 라이브러리를 포함하기 위한 플래그를 설정
  
SRCS: 컴파일할 소스 파일 목록
  
TARGET: 최종적으로 생성할 파일의 이름
```  

# main.cpp

• OpenCV의 TickMeter를 사용하여 각 프레임의 처리 시간을 측정하고, GStreamer와 videocapture, videoWriter를 활용하여 영상을 처리

https://github.com/smHan22/smart-vision/blob/9af135cc8f07717b5c8a865ba745c841ed042b05/jetson%20camera/main.cpp#L1-L55


```
Jetson Nano에서 GStreamer를 사용하여 카메라 영상을 입력받음
          
각 포트로 영상을 전송하기 위해 VideoWriter 객체를 생성
          
영상은 원본, 그레이스케일, 이진화의 세 가지 형식으로 변환되어 각각 writer1, writer2, writer3을 통해 송출
```

# 실행 결과

https://youtu.be/DudBvArEHAU
