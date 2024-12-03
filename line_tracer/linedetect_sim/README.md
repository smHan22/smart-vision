# 동작 영상

➩ 유튜브 링크
https://youtu.be/2eD7gTxhmKs

# 코드 설명

https://github.com/smHan22/smart-vision/blob/f372cbe151428829943ed32c1621077622cab18c/line_tracer/main.cpp#L11-L20

  ```ruby
● 비디오 파일을 열고, 프레임 속도를 설정하는 부분입니다.

● VideoCaputre를 사용하여 비디오 파일을 열고, cap.get(CAP_PROP_FPS)로 비디오의 FPS(초당 프레임 수)를 가져옵니다.

● FPS를 이용하여 delay를 계산하고 각 프레임을 처리하는 시간 간격을 설정합니다.
```

https://github.com/smHan22/smart-vision/blob/43959dff97d8274cf134eedba4f08d541011f169/line_tracer/main.cpp#L22-L28

```ruby
● 영상 처리와 관련된 변수들을 초기화합니다.

● previousCenter는 이전 프레임에서 라인의 중심점을 저장하는 변수이며, firstFrame은 첫 번째 프레임을 처리하는지 확인하는 변수입니다.

● MAX_DISTANCE는 라인의 중심점 간의 최대 허용 거리로 설정되며, start와 edn1은 측정을 위한 변수입니다.

● error는 중앙점과 라인의 중심점 간의 오차를 계산하는 변수입니다.
```

https://github.com/smHan22/smart-vision/blob/f6c86c6a355d03f59dcd9ec9131eb645077c8f81/line_tracer/main.cpp#L29-L45

```ruby
● 비디오에서 한 프레임을 읽고, 그레이스케일로 변환합니다.

● cap >> frame는 비디오에서 한 프레임을 읽어옵니다. 비디오가 끝나면 frame.empty()가 true가 되어 루프를 종료합니다.

● cvtColor(frame, gray, COLOR_BGR2GRAY)는 컬러 영상을 그레이스케일로 변환합니다.

● mean(gray)를 통해 그레이스케일 영상의 평균 밝기를 계산합니다. 이 평균 밝기를 바탕으로 밝기를 조정하여 gray의 밝기를 100에 맞춥니다.

● threshold() 함수는 그레이스케일 영상을 이진화하여 binary로 만듭니다. 밝은 영역은 255(흰색)으로, 어두운 영역은 0(검은색)으로 변환됩니다.

● Rect roi는 (0, 270)dptj (640, 90) 크기로 자릅니다.

● ResizedBinary = binary(roe)로 이진화된 영상에서 정의된 영역을 잘라냅니다.
```

https://github.com/smHan22/smart-vision/blob/83a1647082c41f5e2203509030ede28a1e905121/line_tracer/main.cpp#L47-L67

```ruby
● connectedComponentsWithStats() 함수는 이진화된 영상에서 연결된 객체들을 찾아 라벨을 지정하고, 각 객체의 바운딩 박스와 중심점을 계산합니다.

● labelImage는 각 픽셀의 라벨을 저장하는 영상, stats는 바운딩 박스 정보, centroids는 각 객체의 중심점 정보를 저장합니다.

● 각 라인의 중심점을 계산하고 바운딩 박스를 그립니다.

● 각 라벨에 대해 stats에서 바운딩 박스의 위치와 크기를 가져오고, centroids에서 중심점을 계산합니다.

● lineCenters 백터에 각 라인의 중심점을 추가하고, rectangle()과 circle() 함수로 바운딩 박스와 중심점을 그립니다.

```

https://github.com/smHan22/smart-vision/blob/e699b52950b296793e19cd52ecfcdd0171176b52/line_tracer/main.cpp#L69-L90

```ruby
● if (firstFrame && !lineCenters.empty())는 firstFrame이 ture일 때 즉, 첫 번째 프레임일 때 라인 중심점(lineCenters)이 하나 이상 있는 경우에만 실행됩니다. lineCenter가 비어 있으면 라인을 찾을 수 없기 때문에 라인 추적을 할 수 없습니다.

● Point centerOfImage(frame.cols / 2, frame.rows / 2)는 영상의 중앙 좌표를 계산합니다. 라인 추적의 기준이 되기 위함입니다. 중앙에서 가장 가까운 라인 중심점을 찾고, 그 차이를 이용하여 라인 추적 방향을 결정할 수 있습니다.

● double minDistance = DBL_MAX;는 라인 중심점과 ;이미지 중앙 최소 거리를 추적하기 위한 초기값으로 double 타입에서 가질 수 있는 가장 큰 값으로 설정합니다.

● for (size_t i = 0; i < lineCenters.size(); i++) 이 반복문 내부에서는 각 라인 중심점에 대해, 중앙과 라인 중심점 간의 거리를 계산합니다. if (distance < minDistance) 계산된 거리가 minDistance보다 작으면, 그 중심점을 가장 가까운 중심점으로 선택합니다.

● rectangle(colorBinary, Rect(closestCenter.x - 10, closestCenter.y - 10, 20, 20), Scalar(0, 0, 255), 2)은 가장 가까운 라인 중심점을 빨간색 바운딩 박스와 원으로 표시합니다.

● error 계산과, 현재 라인 중심점을 previousCenter에 저장하여, 다음 프레임에서 추적할 라인을 결정하는 기준으로 사용됩니다.

● 첫 번째 프레임 처리가 완료되었으므로, firstFrame을 false로 설정하여 이후 프레임에서는 라인을 계속 추적할 수 있도록 합니다.
```

https://github.com/smHan22/smart-vision/blob/a720728d50ed1d5904084999e576bdd8e6fb5880/line_tracer/main.cpp#L92-L116

```ruby
● 여기서는 두 번째 프레임부터 라인을 추적하는 과정을 다룹니다. 첫 번째 프레임에서 previousCenter가 설정되었기 때문에, 이후 프레임에서는 이 previousCenter를 기준으로 가장 가까운 라인 중심점을 찾아서 추적합니다.

● else if (previousCenter.x != -1 && !lineCenters.empty()) previousCenter.x가 -1이 아니라는 것은 첫 번째 프레임에서 중심점을 찾았다는 뜻입니다. 또한 lineCenter가 비어있지 않아야 라인 추적을 계속할 수 있습니다. 이 조건이 참일 때만 라인 추적을 계속합니다.

● double minDistance = DBL_MAX; 라인 중심점과 이전 중심점 간의 최소 거리를 추적하기 위한 초기값으로 매우 큰 값으로 설정합니다.

● for (size_t i = 0; i < lineCenters.size(); i++) 반복문 내에서 norm(lineCenters[i] - previousCenter)를 사용하여 이전 중심점과 현재 라인 중심점 간의 거리를 계산합니다.

● if (distance < minDistance && distance < MAX_DISTANCE) 이 조건에서 계산된 거리가 minDistance보다 작고, MAX_DISTANCE보다 작은 경우에만 최소 거리로 저장합니다. MAX_DISTANCE는 너무 멀리 떨어진 라인 중심점은 추적하지 않도록 하는 값입니다.

● 가장 가까운 라인 중심점에 빨간색 바운딩 박스와 원을 그립니다.

● else문에서는 라인이 사라진 경우를 처리합니다. 만약 가까운 라인 중심점이 MAX_DISTANCE보다 멀리 떨어져 있어 추적이 불가능한 경우, 이전 중심점을 그대로 사용하여 해당 위치에 바운딩 박스와 원을 그립니다.

● Point centerOfImage(frame.cols / 2, frame.rows / 2); 영상 중앙의 좌표를 계산하고, error = centerOfImage.x - closestCenter.x; 중앙과 가장 가까운 라인 중심점 간의 x좌표 차이를 계산하여 error값을 업데이트 합니다.

● previousCenter = closestCenter; 현재 찾은 가장 가까운 라인 중심점을 previousCenter에 저장합니다.
```

https://github.com/smHan22/smart-vision/blob/76225e5e26c2b183d94aceba822725a938da0ba5/line_tracer/main.cpp#L118-L127

```ruby
● imshow()는 원본 비디오 프레임과, 바운딩 박스와 라인의 중심점을 표시한 이진화된 이미지를 시각적으로 보여줍니다.

● 조건문은 사용자가 ESC 키를 눌렀을 때 break로 루프를 종료합니다.

● 실행 시간 및 에러를 출력합니다.
```
