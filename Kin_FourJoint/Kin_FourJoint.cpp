#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include <sys/timeb.h>
#include <locale>
#include <string>

#include <atltime.h>

#include <time.h> // C언어 time
#include <ctime> // C++ time
#include <chrono>

#include <k4a/k4a.hpp> // 키넥트 센서 라이브러리
#include <k4abt.hpp> // 키넥트 바디 트래킹 라이브러리
#include <atlstr.h>

#include <direct.h>		//mkdir
#include <errno.h>		//errno

#include <time.h>
#include <sys/timeb.h>
#include <cstdio>
#include <cstring>
#include <ctime>
using namespace std::chrono;

#define neck 3
#define ls 5
#define le 6
#define lw 7
// neck : 목, ls : 왼쪽 어깨, le : 왼쪽 팔꿈치, lw : 왼쪽 손목

using namespace std;
using std::string;

static std::vector<double> kvalues;
// 키넥트 저장용 벡터
static std::vector<double> ivalues;

#pragma region DIKnameRegion

// 들어온 문자열 배열의 길이 확인
int getLength(char str[]) {
    int len = 0;
    // 문자 있는 곳까지 길이 세기
    for (int i = 0; str[i] != '\0'; i++) len++;
    return len;
}

// 먼저 들어온 문자열 배열에 뒤에 들어온 문자열 배열 더하기
char* concat(char strto[], const char strfrom[]) {
    int tlen = getLength(strto);
    int i = 0;
    for (i = 0; strfrom[i] != '\0'; i++) strto[tlen + i] = strfrom[i];
    strto[tlen + i] = '\0';
    return strto;
}

char dir_name[20];
// dir_name : 폴더 이름 char 배열
string dName;
// 폴더 이름 string 형
char kin_fname[256];
// 키넥트 파일이름
char imu_fname[256];
// imu 파일이름

//Makename : 시작시에 초기화 되어 디렉토리 이름, 파일 이름 만드는 함수
void Makename() {
    time_t timer;
    struct tm today;
    timer = time(NULL); // 현재 시각을 초 단위
    localtime_s(&today, &timer); // 초 단위의 시간을 분리
    sprintf_s(dir_name,
        "%02d%02d%02d_%02d%02d%02d", // YYMMDD_HHMMSS
        today.tm_year - 100, // 만약 4자리 연도 쓰고 싶으면 1900 더하면 됨
        today.tm_mon + 1,
        today.tm_mday,
        today.tm_hour,
        today.tm_min,
        today.tm_sec
    );
    //file_name에 YYMMDD_HHMMSS 저장
    sprintf_s(kin_fname, "K_");
    concat(kin_fname, dir_name);
    concat(kin_fname, ".csv");
    // 키넥트에서 들어온 데이터 작성 csv : K_YYMMDD_HHMMSS.csv
    sprintf_s(imu_fname, "I_");
    concat(imu_fname, dir_name);
    concat(imu_fname, ".csv");
}

// csv 저장
void write_csv(std::string filename, std::vector<double> dataset) {
    std::ofstream myFile(filename, ios::app);

    myFile << int(dataset[0]);
    myFile << ",";

    for (int j = 1; j < dataset.size(); j++)
    {
        myFile << dataset[j];
        if (j != dataset.size() - 1) myFile << ","; // 맨 마지막에는 콤마 대신 \n
    }
    myFile << "\n";

    // 파일 종료
    myFile.close();
}
#pragma endregion

char wTime[20];

int LocalMilli() {
    //localtimer
    time_t timer;
    timer = time(NULL);

    struct tm t;
    localtime_s(&t, &timer);

    sprintf_s(wTime,
        "%02d%02d%02d", // YYMMDD_HHMMSS
        t.tm_hour,
        t.tm_min,
        t.tm_sec
    );

    string wt(wTime);

    long long time_last;
    time_last = time(NULL);

    //밀리초 위한 ftimer
    struct timeb t1;
    ftime(&t1);
    time_t ttt = t1.millitm;
    CString tt;
    tt.Format("%03d", int(ttt));
    std::stringstream ss;
    ss << tt;
    std::string ts = wt + ss.str();
    int nts = stoi(ts);
    return nts;
}

int main()
{
    Makename(); // 이름 초기에 초기화
    string dir(dir_name);

    cout << dir;
    string kcsv(kin_fname);
    string icsv(imu_fname);
    // char형을 string 형으로
    dName = "../" + dir + "/";
    string kstop = dName + "stop.txt";
    int nResult = _mkdir(dName.c_str());
    // 상대경로
    std::ofstream kFile(dName + kcsv, ios::app);

    k4a::image rgbImage;
    k4a::image depthImage;

    k4a::transformation transformation;
    std::chrono::microseconds current_time(1000);


    try
    {
        k4a_device_configuration_t device_config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
        device_config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
        device_config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;


        k4a::device device = k4a::device::open(0);
        device.start_cameras(&device_config);



        k4a::calibration sensor_calibration = device.get_calibration(device_config.depth_mode, device_config.color_resolution);
        k4abt::tracker tracker = k4abt::tracker::create(sensor_calibration);
        transformation = k4a::transformation(sensor_calibration);
        int frame_count = 0;
        do
        {
            k4a::capture sensor_capture;
            if (device.get_capture(&sensor_capture, std::chrono::milliseconds(K4A_WAIT_INFINITE)))
            {
                double time_ = static_cast<double>(duration_cast<std::chrono::milliseconds>(system_clock::now().time_since_epoch()).count());
                frame_count++;

                std::cout << "Start processing frame " << frame_count << std::endl;

                if (!tracker.enqueue_capture(sensor_capture))
                {
                    // It should never hit timeout when K4A_WAIT_INFINITE is set.
                    std::cout << "Error! Add capture to tracker process queue timeout!" << std::endl;
                    break;
                }

                k4abt::frame body_frame = tracker.pop_result();
                if (body_frame != nullptr)
                {
                    rgbImage = sensor_capture.get_color_image();
                    uint32_t num_bodies = body_frame.get_num_bodies();
                    std::cout << num_bodies << " bodies are detected!" << std::endl;

                    /*double time_ = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(
                        body_frame.get_device_timestamp()).count());*/


                    for (uint32_t i = 0; i < num_bodies; i++)
                    {
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
                    }

                    k4a::image body_index_map = body_frame.get_body_index_map();
                }
                else
                {
                    //  It should never hit timeout when K4A_WAIT_INFINITE is set.
                    std::cout << "Error! Pop body frame result time out!" << std::endl;
                    break;
                }
            }
            else
            {
                // It should never hit time out when K4A_WAIT_INFINITE is set.
                std::cout << "Error! Get depth frame time out!" << std::endl;
                break;
            }
        } while (frame_count < 100);
        std::cout << "Finished body tracking processing!" << std::endl;
        ofstream HzWriteFile(kstop.c_str());
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed with exception:" << std::endl
            << "    " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
