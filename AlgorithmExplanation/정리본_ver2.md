## 코드 실행
1. main.sln 실행하면 다음과 같이 키넥트와 IMU의 기록이 모두 끝난 후 IMU를 몇개 사용했는 지 입력 받음. (1~2)
![Image](https://i.imgur.com/I9kHtoD.png)

2. 입력하고 나면 1개일 경우,
![Image](https://i.imgur.com/NX430iX.png)
- merged.csv : 단순히 키넥트 데이터와 IMU 데이터 중에서 IMU 데이터 interpolate 후 시간에 맞게 합친 것
- inverse 곱한 비교들.csv
- relative.gif
![relative](https://user-images.githubusercontent.com/48379869/149403777-53693bbf-a3d9-412e-bdfa-c07016ba076d.gif)

3. imu가 2개일 경우,
![Image](https://i.imgur.com/YI2T1w9.png)
- merged.csv : 단순히 키넥트 데이터와 IMU 데이터 중에서 IMU 데이터 interpolate 후 시간에 맞게 합친 것
- inverse 곱한 비교들.csv
- two_imu.gif
![two_imu](https://user-images.githubusercontent.com/48379869/149404080-d42d1dc7-bde0-4348-94f4-bff44ac38711.gif)

+ 여기에 그 다 이어서 그리는 건 빠르게 합쳐서 보내겠습니다!

### C에서 Python.h 사용
![Image](https://i.imgur.com/LkKamCS.png)
프로젝트 속성에서

VC++ 디렉토리에서

파이썬 include와 libs 폴더를

포함, 라이브러리 디렉터리에 각각 추가

![Image](https://i.imgur.com/6n6IQmS.png)

링커 - 입력에서 추가 종속성에 python버전.lib파일 (여기서는 python39.lib 파일) 추가

### farm_hbt 패키지 설치
![Image](https://i.imgur.com/7spvEld.png)
```
아직 몇 개 없는데 정리용으로 자주 쓰는 파이썬 메서드의 경우 그냥 farm_hbt 아래 tools를 따로 파서 패키지로 저장하는 중!

새로 고칠 때마다 아래 python setup.py install 명령해주기
```
1. 위 C에서 사용하는 파이썬 환경에서 터미널로 main 폴더 디렉토리로 이동

2. main 폴더에 setup.py가 있음. `python setup.py install` 명령으로 만든 모듈 해당 버전에서 전역으로(?) 사용할 수 있게 해주기

#### 파이썬 코드 간단하게
- one_imu.py : imu 한 개 일때 interpolate 후 kinect와 imu merge 용으로 만든 파이썬 코드
- two_imu.py : imu가 2개 일 때 위와 동일.
- merged_relative.py : merged.csv에서 필요한 쿼터니언들 연산 및 plot
- two_imu_rel.py : imu 2개 사용할 때 위와 동일.