# 키보드로 방향제어

➨ Dynamixel 모터를 키보드 입력을 통해 실시간으로 제어. 사용자가 f, b, l, r, s 등의 키를 입력하면 모터의 속도와 방향이 즉시 변경됩니다. 이를 통해 전진, 후진, 좌회전, 우회전, 정지 같은 다양한 동작을 수행할 수 있습니다.

# Makefile

• Makefile은 프로젝트의 빌드 과정을 자동화하는 파일로, 여러 파일이 포함된 프로젝트를 쉽게 컴파일하고 관리할 수 있도록 함.

https://github.com/smHan22/smart-vision/blob/66d40174e8040c9bc66abf6f7e6c41bb5e11df4e/Dynamixel%20Programming/project2/Makefile#L1-L15
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


# main.cpp

• main.cpp로 DynamiXel 모터를 제어하고, 키보드 입력을 통해 모터를 제어하면서 실행 시간을 출력하는 프로그램.

https://github.com/smHan22/smart-vision/blob/9c4745d5eb99bfcf6488bb400ba049a66ce8c721/Dynamixel%20Programming/project2/main.cpp#L1-L42
```
• SIGINT 신호가 발생 ( Ctrl+c)했을 때 ctrl_handler 함수가 호출되며, ctrl_c_pressed 변수를 true로 설정합니다. 

• Dxl mx;로 모터 제어 객체를 생성하고, signal(SIGINT, ctrlc_handler);를 통해 Ctrl+c 입력 시 ctrlc_handler가 호출되도록 설정

• vel1과 vel2는 각각 두 모터의 속도를 저장하는 변수.

• mx.open()을 통해 모터와의 연결을 시도함. 실패하면 에러 메시지를 출력하고 프로그램이 종료.

• while 루프 안에서 mx.kbhit() 으로 키보드 입력을 확인하고, 입력이 있으면 mx.getch()로 키를 읽어옴.

• 키보드 입력에 따라 vel1과 vel2의 값이 바뀌며, 이 값이 mx.setVelocity(vel1, vel2);로 전달되어 모터의 속도가 설정됨.

• ctrl_c_pressed가 true이면 루프를 종료하여 프로그램을 끝내고, usleep(20*1000);은 20ms 동안 대기하여 CPU사용을 줄임.

• gettimeofday로 시간을 측정해 time1에 저장하고, 현재 속도와 경과 시간을 출력.

• 루프가 종료되면 mx.close()로 모터와의 연결을 해제하고 프로그램을 종료함.
```

# 실행결과


# 문제점

➨ 정지명령(s입력)을 입력했을 때와 프로그램을 종료했을 때(ctrl+c) 바퀴를 손으로 움직일 때 느껴지는 토크 차이를 설명하라.
```
정지 명령을 입력하면 프로그램이 실행 중이며, 모터는 브레이크 모드로 전기적 저항 토크를 발생시켜 바퀴를 움직이기 어렵게 만듬.
이로 인해 높은 토크가 느껴짐.
반면 프로그램 종료 시 모터 제어 신호가 중단되고 브레이크 모드가 비활성화됨.
이 경우 전기적 저항이 사라지고, 바퀴를 손으로 돌릴 때 낮은 토크만 느껴지는 것.
```

➨ Ctrl+c를 눌렀을때 실습과제1의 결과와 다른 이유를 설명하라.
```
첫 번째 코드는 cin을 사용하여 입력 대기를 하므로, Ctrl+c가 눌려도 입력 대기 상태가 끝나야 종료 조건을 확인할 수 있고,
두 번째 코드는 입력 대기 없이 루프를 계속 진행하므로 Ctrl+c 입력 즉시 종료됨.
```
