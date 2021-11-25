#include <assert.h>
#include <k4a/k4a.hpp>
#include <k4abt.hpp>
#include <cstring>
#include "CsvWriter.hpp"
#include <iostream>

using namespace std;
using namespace csv;

string csvName;
const char *kinDir = "KinData";

void saveCsv() {
	// 파일 이름 생성용
    time_t timer;
    timer = time(NULL);

    struct tm t;
    localtime_s(&t, &timer);

	string whatTime = to_string(t.tm_year + 1900) + "년" +
		to_string(t.tm_mon + 1) + "월" +
		to_string(t.tm_mday) + "일" +
		to_string(t.tm_hour) + "시" +
		to_string(t.tm_min) + "분" +
		to_string(t.tm_sec) + "초"; //현재 시간 string 변수

	csvName = "KinData/" + whatTime + ".csv";
	cout << "file name : " + csvName << endl;
	//파일 이름 정하기만 하는 함수
}

void save_body_information(k4abt_body_t body) {
	CsvWriter writer(csvName);

	for (int i = 0; i < (int)K4ABT_JOINT_COUNT; i++)
	{
		k4a_float3_t position = body.skeleton.joints[i].position;
		k4a_quaternion_t orientation = body.skeleton.joints[i].orientation;

		Record record;
		record.put(position.v[0]);        
		record.put(position.v[1]);        
		record.put(position.v[2]);
		record.put(orientation.v[0]);
		record.put(orientation.v[1]);
		record.put(orientation.v[2]);
		record.put(orientation.v[3]);
		writer.insertRecord(record);
		writer.write();
	}
}

int kSave() {
    try
    {
        saveCsv();
        // * common code
        // kinectDEF : kinect color 카메라 와 depth 카메라, tracker까지 설정, device open(kinect 여러대나 몇 번째냐도 달라질 수 있음) 설정
#pragma region kinectDEF
        k4a_device_configuration_t device_config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
        device_config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
        k4a::device device = k4a::device::open(0);
        device.start_cameras(&device_config);
        k4a::calibration sensor_calibration = device.get_calibration(device_config.depth_mode, device_config.color_resolution);
        k4abt::tracker tracker = k4abt::tracker::create(sensor_calibration);
#pragma endregion

        int frame_count = 0;
        do
        {
            k4a::capture sensor_capture;
            if (device.get_capture(&sensor_capture, std::chrono::milliseconds(K4A_WAIT_INFINITE)))
            {
                frame_count++;
                if (!tracker.enqueue_capture(sensor_capture))
                {
                    // It should never hit timeout when K4A_WAIT_INFINITE is set.
                    std::cout << "Error! Tracker timeout!" << std::endl;
                    break;
                }
                k4abt::frame body_frame = tracker.pop_result();
                if (body_frame != nullptr)
                {
                    uint32_t num_bodies = body_frame.get_num_bodies();
                    for (uint32_t i = 0; i < num_bodies; i++)
                    {
                        k4abt_body_t body = body_frame.get_body(i);
                        save_body_information(body);
                    }
                }
                // * common code
                // Error 처리
#pragma region commonError
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
        std::cout << "Finished Saving!" << std::endl;
    }
    catch (const std::exception& e)
    {
        // try-catch Error
        std::cerr << "Failed with exception:" << std::endl
            << "    " << e.what() << std::endl;
        return 1;
    }
#pragma endregion
    return 0;
}

void kSaveF() {
    kSave();
}