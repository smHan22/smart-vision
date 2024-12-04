# 실행영상

https://youtu.be/uSDSKUINZIs

# 라인트레이서_모션제어

➨ 지난 시간 라인 검출에 이어서 검출한 라인과 위치오차를 이용하여 속도명령을 설계하는 것을 설명하겠습니다.

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
