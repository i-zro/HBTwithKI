- [SDK](#sdk)
    - [k4abttypes.h 정의 내용](#k4abttypesh-정의-내용)
      - [목은 3](#목은-3)
      - [왼쪽 어깨는 5](#왼쪽-어깨는-5)
      - [왼쪽 팔꿈치는 6](#왼쪽-팔꿈치는-6)
      - [왼쪽 손목은 7](#왼쪽-손목은-7)
- [C++ 데이터 받기](#c-데이터-받기)
  - [초기 설정](#초기-설정)
  - [Kinect 4개 관절 좌표 받아서 위치 시각화](#kinect-4개-관절-좌표-받아서-위치-시각화)
    - [Kinect에서 4개 받아서 csv 저장 주요 코드](#kinect에서-4개-받아서-csv-저장-주요-코드)
    - [kinect csv 위치 좌표만 시각화](#kinect-csv-위치-좌표만-시각화)
      - [line 진행중 (아래처럼 나와서 새로 갱신되는? 코드 다시 짜야함.)](#line-진행중-아래처럼-나와서-새로-갱신되는-코드-다시-짜야함)
- [파이썬 시각화](#파이썬-시각화)
    - [FuncAnimation](#funcanimation)
  - [kinect plot 2개 짜는 과정 (애니메이션 2개 동시 plot 테스트용)](#kinect-plot-2개-짜는-과정-애니메이션-2개-동시-plot-테스트용)
  - [kinect, imu relative 동시 plot 코드](#kinect-imu-relative-동시-plot-코드)
  - [kinect relative, imu relative, kinect와 imu의 relative](#kinect-relative-imu-relative-kinect와-imu의-relative)
- [x축, y축, z축 일치 시켜 보기](#x축-y축-z축-일치-시켜-보기)
    - [키넥트](#키넥트)
    - [IMU](#imu)
- [참고](#참고)

# SDK

### k4abttypes.h 정의 내용
- k4abt_joint_id_t
#### 목은 3
#### 왼쪽 어깨는 5
#### 왼쪽 팔꿈치는 6
#### 왼쪽 손목은 7
![Image](https://i.imgur.com/yJEK86j.png)

| 인덱스 | 관절 이름      | 부모 관절      |
|--------|----------------|----------------|
| 0      | PELVIS         | -              |
| 1      | SPINE_NAVAL    | PELVIS         |
| 2      | SPINE_CHEST    | SPINE_NAVAL    |
| 3      | NECK           | SPINE_CHEST    |
| 4      | CLAVICLE_LEFT  | SPINE_CHEST    |
| 5      | SHOULDER_LEFT  | CLAVICLE_LEFT  |
| 6      | ELBOW_LEFT     | SHOULDER_LEFT  |
| 7      | WRIST_LEFT     | ELBOW_LEFT     |
| 8      | HAND_LEFT      | WRIST_LEFT     |
| 9      | HANDTIP_LEFT   | HAND_LEFT      |
| 10     | THUMB_LEFT     | WRIST_LEFT     |
| 11     | CLAVICLE_RIGHT | SPINE_CHEST    |
| 12     | SHOULDER_RIGHT | CLAVICLE_RIGHT |
| 13     | ELBOW_RIGHT    | SHOULDER_RIGHT |
| 14     | WRIST_RIGHT    | ELBOW_RIGHT    |
| 15     | HAND_RIGHT     | WRIST_RIGHT    |
| 16     | HANDTIP_RIGHT  | HAND_RIGHT     |
| 17     | THUMB_RIGHT    | WRIST_RIGHT    |
| 18     | HIP_LEFT       | PELVIS         |
| 19     | KNEE_LEFT      | HIP_LEFT       |
| 20     | ANKLE_LEFT     | KNEE_LEFT      |
| 21     | FOOT_LEFT      | ANKLE_LEFT     |
| 22     | HIP_RIGHT      | PELVIS         |
| 23     | KNEE_RIGHT     | HIP_RIGHT      |
| 24     | ANKLE_RIGHT    | KNEE_RIGHT     |
| 25     | FOOT_RIGHT     | ANKLE_RIGHT    |
| 26     | HEAD           | NECK           |
| 27     | NOSE           | HEAD           |
| 28     | EYE_LEFT       | HEAD           |
| 29     | EAR_LEFT       | HEAD           |
| 30     | EYE_RIGHT      | HEAD           |
| 31     | EAR_RIGHT      | HEAD           |

```cpp
typedef enum
{
    K4ABT_JOINT_PELVIS = 0,
    K4ABT_JOINT_SPINE_NAVEL,
    K4ABT_JOINT_SPINE_CHEST,
    K4ABT_JOINT_NECK,
    K4ABT_JOINT_CLAVICLE_LEFT,
    K4ABT_JOINT_SHOULDER_LEFT,
    K4ABT_JOINT_ELBOW_LEFT,
    K4ABT_JOINT_WRIST_LEFT,
    K4ABT_JOINT_HAND_LEFT,
    K4ABT_JOINT_HANDTIP_LEFT,
    K4ABT_JOINT_THUMB_LEFT,
    K4ABT_JOINT_CLAVICLE_RIGHT,
    K4ABT_JOINT_SHOULDER_RIGHT,
    K4ABT_JOINT_ELBOW_RIGHT,
    K4ABT_JOINT_WRIST_RIGHT,
    K4ABT_JOINT_HAND_RIGHT,
    K4ABT_JOINT_HANDTIP_RIGHT,
    K4ABT_JOINT_THUMB_RIGHT,
    K4ABT_JOINT_HIP_LEFT,
    K4ABT_JOINT_KNEE_LEFT,
    K4ABT_JOINT_ANKLE_LEFT,
    K4ABT_JOINT_FOOT_LEFT,
    K4ABT_JOINT_HIP_RIGHT,
    K4ABT_JOINT_KNEE_RIGHT,
    K4ABT_JOINT_ANKLE_RIGHT,
    K4ABT_JOINT_FOOT_RIGHT,
    K4ABT_JOINT_HEAD,
    K4ABT_JOINT_NOSE,
    K4ABT_JOINT_EYE_LEFT,
    K4ABT_JOINT_EAR_LEFT,
    K4ABT_JOINT_EYE_RIGHT,
    K4ABT_JOINT_EAR_RIGHT,
    K4ABT_JOINT_COUNT
} k4abt_joint_id_t;
```
# C++ 데이터 받기
## 초기 설정
![Image](https://i.imgur.com/4UtjGat.png)
프로젝트 속성에서 고급 - 문자 집합 - 멀티바이트 문자 집합으로 변경 (파일명 저장 시 hhmmss Format 하기 위함.)

## Kinect 4개 관절 좌표 받아서 위치 시각화
### Kinect에서 4개 받아서 csv 저장 주요 코드
https://github.com/i-zro/HBTwithKI/blob/main/Kin_FourJoint/Kin_FourJoint.cpp
```cpp
#define neck 3
#define ls 5
#define le 6
#define lw 7
// neck : 목, ls : 왼쪽 어깨, le : 왼쪽 팔꿈치, lw : 왼쪽 손목
.
.
.
                    k4abt_body_t body = body_frame.get_body(i);
                        kvalues.clear();
                        kvalues.push_back(LocalMilli());
                        // neck
                        k4a_float3_t neckpos = body.skeleton.joints[neck].position;
                        k4a_quaternion_t neckori = body.skeleton.joints[neck].orientation;
                        k4abt_joint_confidence_level_t neckconf = body.skeleton.joints[neck].confidence_level;

                        kvalues.push_back(neck);kvalues.push_back(neckpos.v[0]);kvalues.push_back(neckpos.v[1]);kvalues.push_back(neckpos.v[2]); // 관절좌표 위치부터 신뢰도까지 담기
                        kvalues.push_back(neckori.v[0]);kvalues.push_back(neckori.v[1]);kvalues.push_back(neckori.v[2]);kvalues.push_back(neckori.v[3]);
                        kvalues.push_back(neckconf);

                        // left shoulder
                        k4a_float3_t lspos = body.skeleton.joints[ls].position;
                        k4a_quaternion_t lsori = body.skeleton.joints[ls].orientation;
                        k4abt_joint_confidence_level_t lsconf = body.skeleton.joints[ls].confidence_level;

                        kvalues.push_back(ls);kvalues.push_back(lspos.v[0]);kvalues.push_back(lspos.v[1]);kvalues.push_back(lspos.v[2]); // 관절좌표 위치부터 신뢰도까지 담기
                        kvalues.push_back(lsori.v[0]);kvalues.push_back(lsori.v[1]);kvalues.push_back(lsori.v[2]);kvalues.push_back(lsori.v[3]);
                        kvalues.push_back(lsconf);

                        // left elbow
                        k4a_float3_t lepos = body.skeleton.joints[le].position;
                        k4a_quaternion_t leori = body.skeleton.joints[le].orientation;
                        k4abt_joint_confidence_level_t leconf = body.skeleton.joints[le].confidence_level;

                        kvalues.push_back(le);kvalues.push_back(lepos.v[0]);kvalues.push_back(lepos.v[1]);kvalues.push_back(lepos.v[2]); // 관절좌표 위치부터 신뢰도까지 담기
                        kvalues.push_back(leori.v[0]);kvalues.push_back(leori.v[1]);kvalues.push_back(leori.v[2]);kvalues.push_back(leori.v[3]);
                        kvalues.push_back(leconf);

                        // left wrist
                        k4a_float3_t lwpos = body.skeleton.joints[lw].position;
                        k4a_quaternion_t lwori = body.skeleton.joints[lw].orientation;
                        k4abt_joint_confidence_level_t lwconf = body.skeleton.joints[lw].confidence_level;

                        kvalues.push_back(lw);kvalues.push_back(lwpos.v[0]);kvalues.push_back(lwpos.v[1]);kvalues.push_back(lwpos.v[2]); // 관절좌표 위치부터 신뢰도까지 담기
                        kvalues.push_back(lwori.v[0]);kvalues.push_back(lwori.v[1]);kvalues.push_back(lwori.v[2]);kvalues.push_back(lwori.v[3]);
                        kvalues.push_back(lwconf);
                        write_csv(dName + kcsv, kvalues);
```
: 시간, 관절 번호, 위치 좌표들 3개(xyz), orientation 좌표들 4개(wxyz), 신뢰도

![Image](https://i.imgur.com/fmj4Iu3.png)

[🧾csvFile](https://github.com/i-zro/HBTwithKI/blob/main/csv_files/K_220112_012632.csv)

### kinect csv 위치 좌표만 시각화
https://github.com/i-zro/HBTwithKI/blob/main/python/plot_joints.py
```python
# kinect csv 불러와서 데이터프레임으로
def k_to_df():
    k_csv = 'K*.csv'
    kinfile = glob.glob(k_csv)
    kin_df = pd.read_csv(kinfile[0], names = ['t', 'neck', 'neckx', 'necky', 'neckz',
                                            'neckww', 'neckwx', 'neckwy', 'neckwz', 'neckconf',
                                            'ls', 'lsx', 'lsy', 'lsz',
                                            'lsww', 'lswx', 'lswy', 'lswz', 'lsconf',
                                            'le', 'lex', 'ley', 'lez',
                                            'leww', 'lewx', 'lewy', 'lewz', 'leconf',
                                            'lw', 'lwx', 'lwy', 'lwz',
                                            'lwww', 'lwwx', 'lwwy', 'lwwz', 'lwconf'
                                            ])
    return kin_df
kin_df = k_to_df()
# print(kin_df)
```
![plot_joints](https://user-images.githubusercontent.com/48379869/149237028-16844a35-9c2f-445e-aeec-dcac1b1fafbd.gif)

#### line 진행중 (아래처럼 나와서 새로 갱신되는? 코드 다시 짜야함.)
![plot_joints](https://user-images.githubusercontent.com/48379869/149237154-45646511-be0a-4ca5-b1be-30dc73da0ef0.gif)

# 파이썬 시각화
### FuncAnimation
- interval을 사용해서 프레임 사이의 간격을 밀리초 (milliseconds) 단위로 지정할 수 있다. 디폴트 값은 200

## kinect plot 2개 짜는 과정 (애니메이션 2개 동시 plot 테스트용)
```python
import matplotlib
matplotlib.use("TkAgg") #팝업플로팅에 사용

import matplotlib.pyplot as plt

from spatialmath import * # UnitQuaternion
from spatialmath.pose3d import *

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation
from matplotlib import cm

import numpy as np

from mpl_toolkits.mplot3d.axes3d import get_test_data

import os, glob
import os.path

import csv
```
```python
### part 0.
k_csv = "krelative.csv"
krow = [] # kinect용 배열

def csv_read(file_name, arow, first, end): # 파일 이름, 받아올 리스트, 시작열번호, 끝열번호
    with open(file_name, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        fields = next(csvreader)

        for row in csvreader:
            arow.append(list(map(float, row[first:end])))

csv_read(k_csv, krow, 1, 5) # csv 파일 불러오기
print(krow)
```
![Image](https://i.imgur.com/eJBCBX0.png)

: krelative 들어온 것 print

```python
### part 1.

fig = plt.figure(figsize=plt.figaspect(0.5))
#첫 subplot
ax = fig.add_subplot(1, 2, 1, projection='3d')

ax.set_xlim(-0.5, 0.5)
ax.set_ylim(-0.5, 0.5)
ax.set_zlim(0, 1)

axes = [ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0)]
axes2 = [ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0)]

colors = ['#E16F6D', '#4eb604', '#0d87c3'] # RGB 색 hex 값

def update(count):
    row=krow[count]
    q=UnitQuaternion([float(row[3]), float(row[0]), float(row[1]),float(row[2])]).R

    for i in range(3):
        axes[i].remove()
        axes[i] = ax.quiver(0, 0, 0.5, q[0][i], q[1][i], q[2][i],
                            length=0.1, normalize=True, color=colors[i])
def update2(count):
    row=krow[count]
    q=UnitQuaternion([float(row[3]), float(row[0]), float(row[1]),float(row[2])]).R

    for i in range(3):
        axes2[i].remove()
        axes2[i] = ax2.quiver(0, 0, 0.5, q[0][i], q[1][i], q[2][i],
                            length=0.1, normalize=True, color=colors[i])

ani = FuncAnimation(fig, # fig에 그릴 것.
                    update,
                    frames=range(0, len(krow)), interval=100,
                    repeat=False)

ax2 = fig.add_subplot(1, 2, 2, projection='3d')

ax2.set_xlim(-0.5, 0.5)
ax2.set_ylim(-0.5, 0.5)
ax2.set_zlim(0, 1)

ani2 = FuncAnimation(fig, # fig에 그릴 것.
                    update2,
                    frames=range(0, len(krow)), interval=100,
                    repeat=False)

plt.show()
```
![Figure_1](https://user-images.githubusercontent.com/48379869/148966320-61bab12a-012f-4492-9053-2449f5d6ffb2.png)

## kinect, imu relative 동시 plot 코드
```python
import matplotlib
matplotlib.use("TkAgg") #팝업플로팅에 사용

import matplotlib.pyplot as plt

from spatialmath import * # UnitQuaternion
from spatialmath.pose3d import *

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation
from matplotlib import cm

import numpy as np

from mpl_toolkits.mplot3d.axes3d import get_test_data

import os, glob
import os.path

import csv

### part 0. krelative 받아오기

k_csv = "krelative.csv"
krow = [] # kinect용 배열
i_csv = "irelative.csv"
irow = [] # imu용 배열

def csv_read(file_name, arow, first, end): # 파일 이름, 받아올 리스트, 시작열번호, 끝열번호
    with open(file_name, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        fields = next(csvreader)

        for row in csvreader:
            arow.append(list(map(float, row[first:end])))

csv_read(k_csv, krow, 1, 5) # csv 파일 불러오기
csv_read(i_csv, irow, 2, 6)
#print(krow)
#print(irow)
### part 1.

fig = plt.figure(figsize=plt.figaspect(0.5))
#첫 subplot
ax = fig.add_subplot(1, 2, 1, projection='3d')

ax.set_xlim(-0.5, 0.5)
ax.set_ylim(-0.5, 0.5)
ax.set_zlim(0, 1)

axes = [ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0)]
axes2 = [ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0)]

colors = ['#E16F6D', '#4eb604', '#0d87c3'] # RGB 색 hex 값

def update(count):
    row=krow[count]
    q=UnitQuaternion([float(row[0]), float(row[1]), float(row[2]),float(row[3])]).R

    for i in range(3):
        axes[i].remove()
        axes[i] = ax.quiver(0, 0, 0.5, q[0][i], q[1][i], q[2][i],
                            length=0.1, normalize=True, color=colors[i])
def update2(count):
    row=irow[count]
    q=UnitQuaternion([float(row[0]), float(row[1]), float(row[2]),float(row[3])]).R

    for i in range(3):
        axes2[i].remove()
        axes2[i] = ax2.quiver(0, 0, 0.5, q[0][i], q[1][i], q[2][i],
                            length=0.1, normalize=True, color=colors[i])

ani = FuncAnimation(fig, # fig에 그릴 것.
                    update,
                    frames=range(0, len(krow)), interval=100,
                    repeat=False)

ax2 = fig.add_subplot(1, 2, 2, projection='3d')

ax2.set_xlim(-0.5, 0.5)
ax2.set_ylim(-0.5, 0.5)
ax2.set_zlim(0, 1)

ani2 = FuncAnimation(fig, # fig에 그릴 것.
                    update2,
                    frames=range(0, len(irow)), interval=100,
                    repeat=False)

plt.show()
```
![Image](https://i.imgur.com/uvOUgAR.png)

## kinect relative, imu relative, kinect와 imu의 relative
```python
import matplotlib
matplotlib.use("TkAgg") #팝업플로팅에 사용

import matplotlib.pyplot as plt

from spatialmath import * # UnitQuaternion
from spatialmath.pose3d import *

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation
from matplotlib import cm

import numpy as np

from mpl_toolkits.mplot3d.axes3d import get_test_data

import os, glob
import os.path

import csv

### part 0. krelative 받아오기

k_csv = "krelative.csv"
krow = [] # kinect용 배열
i_csv = "irelative.csv"
irow = [] # imu용 배열
rel_csv = "relative.csv"
relrow = [] # imu용 배열

def csv_read(file_name, arow, first, end): # 파일 이름, 받아올 리스트, 시작열번호, 끝열번호
    with open(file_name, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        fields = next(csvreader)

        for row in csvreader:
            arow.append(list(map(float, row[first:end])))

csv_read(k_csv, krow, 1, 5) # csv 파일 불러오기
csv_read(i_csv, irow, 2, 6)
csv_read(rel_csv, relrow, 2, 6)
#print(krow)
#print(irow)
print(relrow)
### part 1.

fig = plt.figure(figsize=plt.figaspect(0.5))
#첫 subplot
ax = fig.add_subplot(1, 3, 1, projection='3d')

ax.set_xlim(-0.5, 0.5)
ax.set_ylim(-0.5, 0.5)
ax.set_zlim(0, 1)

axes = [ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0)]
axes2 = [ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0)]
axes3 = [ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0)]

colors = ['#E16F6D', '#4eb604', '#0d87c3'] # RGB 색 hex 값

def update(count):
    row=krow[count]
    q=UnitQuaternion([float(row[0]), float(row[1]), float(row[2]),float(row[3])]).R

    for i in range(3):
        axes[i].remove()
        axes[i] = ax.quiver(0, 0, 0.5, q[0][i], q[1][i], q[2][i],
                            length=0.1, normalize=True, color=colors[i])
def update2(count):
    row=irow[count]
    q=UnitQuaternion([float(row[0]), float(row[1]), float(row[2]),float(row[3])]).R

    for i in range(3):
        axes2[i].remove()
        axes2[i] = ax2.quiver(0, 0, 0.5, q[0][i], q[1][i], q[2][i],
                            length=0.1, normalize=True, color=colors[i])

def update3(count):
    row=relrow[count]
    q=UnitQuaternion([float(row[0]), float(row[1]), float(row[2]),float(row[3])]).R

    for i in range(3):
        axes3[i].remove()
        axes3[i] = ax3.quiver(0, 0, 0.5, q[0][i], q[1][i], q[2][i],
                            length=0.1, normalize=True, color=colors[i])

ani = FuncAnimation(fig, # fig에 그릴 것.
                    update,
                    frames=range(0, len(krow)), interval=100,
                    repeat=False)

ax2 = fig.add_subplot(1, 3, 2, projection='3d')

ax2.set_xlim(-0.5, 0.5)
ax2.set_ylim(-0.5, 0.5)
ax2.set_zlim(0, 1)

ani2 = FuncAnimation(fig, # fig에 그릴 것.
                    update2,
                    frames=range(0, len(irow)), interval=100,
                    repeat=False)

ax3 = fig.add_subplot(1, 3, 3, projection='3d')

ax3.set_xlim(-0.5, 0.5)
ax3.set_ylim(-0.5, 0.5)
ax3.set_zlim(0, 1)

ani3 = FuncAnimation(fig, # fig에 그릴 것.
                    update3,
                    frames=range(0, len(relrow)), interval=100,
                    repeat=False)

plt.show()
```
![Image](https://i.imgur.com/MPTQEMK.png)

# x축, y축, z축 일치 시켜 보기
### 키넥트
- x 축 = 빨강 | y 축 = 녹색 | z 축 = blue
![Image](https://i.imgur.com/I4pgG6J.png)

### IMU


# 참고
https://hansonminlearning.tistory.com/30
plot 여러 팝업창으로 plot

https://jehyunlee.github.io/2021/07/10/Python-DS-80-mpl3d2/
회전

https://github.com/yysijie/st-gcn/blob/master/OLD_README.md#kinetics-skeleton
gcN

https://github.com/IICNELAB/qpu_code/blob/c0eecf2223c3c28d048d816fd239c118b8568dcf/utils/quaternion.py
quaternion

https://stackoverflow.com/questions/35084588/how-to-scale-3d-quiver-diagrams-correctly-in-python-matplotlib
quiver