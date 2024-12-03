# 속도 명령 받기

➨ Jetson Nano를 이용하여 Dynamixel 모터를 제어하고, 사용자가 속도를 수동으로 조정할 수 있는 시스템 구현
```
• 프로그램이 실행되면, Dxl 객체가 생성되고, Dynamixel 모터를 제어하기 위해 mx.open()을 통해 모터 장치를 열음.
(이때 장치 열기에 실패하면 오류 메시지를 출력하고 프로그램이 종료)

• signal(SIGINT, ctrlc)를 사용하여 Ctrl+c 입력을 감지. Ctrl+c가 입력되면, ctrlc함수가 호출되고, 이 함수는 ctrl_c_pressed 플래그를 true로 설정하여 프로그램이 안전하게 종료될 수 있도록 함.

• 무한 루프 안에서 사용자에게 왼쪽과 오른쪽 모터 속도를 입력받고, 사용자가 입력한 값은 mx.setVelocity()를 통해 모터에 전달되어 모터 속도가 설정됨.

• 속도 입력 후, gettimeofday를 사용하여 현재 시간을 측정하고, 이전 시간과의 차이를 계산하여 프로그램 실행 시간을 출력함. 이를 통해 사용자는 모터 속도 입력 후, 해당 동작에 소요된 시간 확인 가능.

• usleep(20*1000)를 사용하여 20밀리초 동안 잠시 대기하며, 이는 모터 속도 업데이트 주기를 설정하는 역할을 함.

• 무한 루프 안에서 계속 실행되며, Ctrl+c가 입력되면, ctrl_c_pressed플래그가 true로 설정되어 루프가 종료되고, 이 시점에서 모터 제어를 위한 장치 연결이 끊어지고, mx.close()가 호출되어 장치가 닫히게 됨.
```
# Makefile 설정

• Makefile은 프로젝트의 빌드 과정을 자동화하는 파일로, 여러 파일이 포함된 프로젝트를 쉽게 컴파일하고 관리할 수 있도록 함.

https://github.com/smHan22/smart-vision/blob/737707ae92b0c697ed4b5529743b4d9e72359cc6/Dynamixel%20Programming/project1/Makefile#L1-L15
```
• TARGET = dxl : 빌드할 최종 실행 파일의 이름을 dxl로 정의합니다.
        
• OBJS = main.o dxl.o : 컴파일된 객체 파일들을 나열. main.o와 dxl.o가 객체 파일
        
• $(TARGET) : $(OBJS): 최종 목표인 dxl을 생성하는 규칙. main.o와 dxl.o 객체 파일에 의존.
        
• $(CX) -o $(TARGET) $(OBJS) $(DXLFLAGS): (CX)는 C++ 컴파일러, -o $(TARGET)는 출력 파일을 dxl로 지정하는 명령. $(OBJS)          는 객체 파일들을 지정하고, $(DXLFLAGS)는 Dynamixel SDK의 포함 경로와 라이브러리 링크를 추가하는 플래그.

• main.o : main.cpp dxl.hpp: main.o를 생성하는 규칙. 

• $(CX) $(CXFLAGS) -c main.cpp $(DXLFLAGS): main.cpp 파일을 컴파일하여 main.o 객체 파일을 생성. $(CXFLAGS)는 컴파일 시의           플래그로, 디버깅 정보를 포함하고 경고 메시지를 출력.

• dxl.o : dxl.hpp dxl.cpp: dxl.o 파일을 생성하는 규칙.

• $(CX) $(CXFLAGS) -c dxl.cpp $(DXLFLAGS): dxl.cpp 파일을 컴파일하여 dxl.o 객체 파일을 생성.
```
# main.cpp 코드

• main.cpp로 Dynamixel 모터를 제어하고, 사용자가 입력한 속도로 모터를 제어하면서 실행 시간을 출력하는 프로그램.

https://github.com/smHan22/smart-vision/blob/6369efbc39c2e7ff26ad6c764d58d5d69b9d1654/Dynamixel%20Programming/project1/main.cpp#L1-L37
```
• Dxl mx; 를 통해 dxl 클래스의 객체를 생성하고, Dynamixel 모터를 제어할 준비를 함.

• signal(SIGINT, ctrlc);로 Ctrl+c 신호를 감지하는 시그널 핸들러를 설정. Ctrl+c가 눌리면 ctrl_c_pressed 변수를 true로 설정           하여 프로그램이 종료될 수 있도록 함.
        
• mx.open()을 호출하여 Dynamixel 모터 장치와 연결. 연결에 실패하면 오류 메시지를 출력하고 프로그램을 종료.

• 무한 루프 내에서 사용자가 왼쪽과 오른쪽 모터 속도를 입력. 입력된 속도 값은 mx.setVelocity(leftspeed, rightspeed)를 통해            모터에 전달되어 모터의 속도가 설정됨.

• gettimeofday($start, NULL); 를 사용해 시작 시간을 기록함. 모터 속도를 설정한 후, usleep(20*1000);으로 20ms 동안 잠시 대           기. 대기 후, gettimeofday(&end1, NULL);를 통해 종료 시간을 기록하고, 시작 시간과 종료 시간을 비교하여 소요된 실행 시간을            계산.

• Ctrl+c를 눌러 시그널을 보내면 ctrl_c_pressed가 true로 설정되어 루프를 종료함. 프로그램이 종료될 때 mx.close();로 모터 장           치와의 연결을 종료.
```
# 실행 결과

https://youtu.be/eUW3GhiBMKs

# 문제점

➨ Ctrl+c를 누르면 바로 종료하지 않고 명령을 한번 더 입력하면 종료하는데 이유를 설명하라. 이걸 해결하려면 어떻게 해야 하는지 나중에 생각해볼 것
```
• 코드에서는 Ctrl+C를 누르면 ctrlc 함수가 호출되고, 이 함수는 ctrl_c_pressed 값을 true로 설정합니다.
하지만 ctrl_c_pressed 값을 체크하는 조건이 루프 내에서 바로 break되도록 하는 것이 아니라, 루프가 끝날 때까지 대기하고 있기 때문.
즉, Ctrl+C가 눌려도 그 즉시 break가 발생하지 않고, 사용자가 명령을 한 번 더 입력한 후에 루프가 종료.
```
➨ 입력한 속도 명령값이 현재 모터 속도와 차이가 클 때 급가속 또는 급감속이 발생하여 진동과 소음이 발생한다. 이를 해결하는 방법을 설명하라.
```
• 모터에 급격한 가속이나 감속을 적용하면 진동과 소음이 발생할 수 있음.
모터가 설정된 속도에 갑작스럽게 도달하려 할 때 발생하는 문제로, 모터의 기계적인 특성과 전자적인 제어 방식에서 발생할 수 있는 현상임.
급격한 가속과 감속을 방지하는 가장 효과적인 방법은 속도의 점진적인 변화를 적용하는 것.
```
