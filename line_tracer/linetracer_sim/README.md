# 실행영상

https://youtu.be/uSDSKUINZIs

# 라인트레이서_모션제어

➨ 지난 시간 라인 검출에 이어서 검출한 라인과 위치오차를 이용하여 속도명령을 설계하는 것을 설명하겠습니다.

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

# vision.hpp

# Makefile


⦁ 모터와 연결

https://github.com/smHan22/smart-vision/blob/050106fa565d134d256f365c2debadaf6123141e/line_tracer/linetracer_sim/main.cpp#L48-L51

```ruby
Dynamixel 모터와의 연결을 시도. dxl.open() 함수가 실패하면 false를 반환

연결이 실패하면 오류 메시지 출력
```

⦁ 에러 계산

https://github.com/smHan22/smart-vision/blob/f10042d2c82ec4e662953af3035686868e0f5b8c/line_tracer/linetracer_sim/main.cpp#L135

```ruby
centerOfImage.x는 화면의 중심 x좌표

closestCenter.x는 현재 라인의 중심점 x좌표

error는 화면 중심과 라인의 중심 간의 x축 거리 차이를 나타냅니다.

값이 양수이면 라인이 화면 중앙보다 왼쪽에 위치, 값이 음수이면 라인이 화면 중앙보다 오른쪽에 위치, 값이 0이면 라인이 화면 중앙에 정확히 위치
```

⦁ 바퀴 속도 계산

https://github.com/smHan22/smart-vision/blob/11ae7d0a7293dad3a580a871046a4447bb1a223d/line_tracer/linetracer_sim/main.cpp#L138-L139

```ruby
leftvel은 왼쪽 바퀴의 속도, rightvel은 오른쪽 바퀴의 속도.

k 게인 값으로 에러에 따라 속도 조정 정도를 설정.

에러가 클수록 바퀴 속도 차이가 커지며, 빠르게 방향을 수정하게 됨.

error > 0 이면 왼쪽 바퀴 속도가 줄고, 오른쪽 바퀴 속도가 늘어나 오른쪽으로 회전

error < 0 이면 오른쪽 바퀴 속도가 줄고, 왼쪽 바퀴 속도가 늘어나 왼쪽으로 회전

error == 0이면 양쪽 바퀴 속도가 동일하여 직진
```

⦁ 모션 제어 실행

https://github.com/smHan22/smart-vision/blob/31e80899364948857daec70b7da19d1b94195e27/line_tracer/linetracer_sim/main.cpp#L150

```ruby
mode가 true인 경우에만 모터 속도를 설정.

's'키를 입력하면 mode = true로 변경되어 모션 제어가 활성화.

'q' 키를 입력하면 프로그램 종료.

dxl.setVelocity(leftvel, rightvel)은 계산된 속도를 기반으로 모터를 제어하여 방향 및 이동 속도를 설정.
```
