#include <assert.h>
#include <k4a/k4a.hpp>
#include <k4abt.hpp>

#include<iostream>
#include<cstring>
#include "kPrinter.h"
#include "KITOOLs.h"
#include <list>

using namespace std;

string printJoint(int num)
{
    string joint; // 관절 좌표 이름
    if (num == 0) { joint = "PELVIS"; }
    else if (num == 1) { joint = "SPINE_NAVEL"; }
    else if (num == 2) { joint = "SPINE_CHEST"; }
    else if (num == 3) { joint = "NECK"; }
    else if (num == 4) { joint = "CLAVICLE_LEFT"; }
    else if (num == 5) { joint = "SHOULDER_LEFT"; }
    else if (num == 6) { joint = "ELBOW_LEFT"; }
    else if (num == 7) { joint = "WRIST_LEFT"; }
    else if (num == 8) { joint = "HAND_LEFT"; }
    else if (num == 9) { joint = "HANDTIP_LEFT"; }
    else if (num == 10) { joint = "THUMB_LEFT"; }
    else if (num == 11) { joint = "CLAVICLE_RIGHT"; }
    else if (num == 12) { joint = "SHOULDER_RIGHT"; }
    else if (num == 13) { joint = "ELBOW_RIGHT"; }
    else if (num == 14) { joint = "WRIST_RIGHT"; }
    else if (num == 15) { joint = "HAND_RIGHT"; }
    else if (num == 16) { joint = "HANDTIP_RIGHT"; }
    else if (num == 17) { joint = "THUMB_RIGHT"; }
    else if (num == 18) { joint = "HIP_LEFT"; }
    else if (num == 19) { joint = "KNEE_LEFT"; }
    else if (num == 20) { joint = "ANKLE_LEFT"; }
    else if (num == 21) { joint = "FOOT_LEFT"; }
    else if (num == 22) { joint = "HIP_RIGHT"; }
    else if (num == 23) { joint = "KNEE_RIGHT"; }
    else if (num == 24) { joint = "ANKLE_RIGHT"; }
    else if (num == 25) { joint = "FOOT_RIGHT"; }
    else if (num == 26) { joint = "HEAD"; }
    else if (num == 27) { joint = "NOSE"; }
    else if (num == 28) { joint = "EYE_LEFT"; }
    else if (num == 29) { joint = "EAR_LEFT"; }
    else if (num == 30) { joint = "EYE_RIGHT"; }
    else if (num == 31) { joint = "EAR_RIGHT"; }
    return joint;
}

void print_body_information(k4abt_body_t body)
{
    std::cout << "Body ID: " << body.id << std::endl;
    for (int i = 0; i < (int)K4ABT_JOINT_COUNT; i++)
    {
        k4a_float3_t position = body.skeleton.joints[i].position;
        k4a_quaternion_t orientation = body.skeleton.joints[i].orientation;
        cout << printJoint(i);
        printf(" : Position[mm] ( %f, %f, %f ); Orientation ( %f, %f, %f, %f); \n",
            position.v[0], position.v[1], position.v[2], orientation.v[0], orientation.v[1], orientation.v[2], orientation.v[3]);
    }
}

int kPrint() {
    try
    {
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

                // frame 처리 시작
                std::cout << "Start" << frame_count << std::endl;

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
                    // 몇 명 인지
                    std::cout << num_bodies << " Bodies " << std::endl;
                    for (uint32_t i = 0; i < num_bodies; i++)
                    {
                        k4abt_body_t body = body_frame.get_body(i);
                        print_body_information(body);
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
        std::cout << "Finished body tracking processing!" << std::endl;
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

void kPrintF() {
    kPrint();
}

void ktoL() {
    list<kLST> klist;
    try
    {
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

                // frame 처리 시작
                std::cout << "Start" << frame_count << std::endl;

                if (!tracker.enqueue_capture(sensor_capture))
                {
                    // It should never hit timeout when K4A_WAIT_INFINITE is set.
                    std::cout << "Error! Tracker timeout!" << std::endl;
                    break;
                }

                k4abt::frame body_frame = tracker.pop_result();
                int time_s;
                if (body_frame != nullptr)
                {
                    
                    uint32_t num_bodies = body_frame.get_num_bodies();
                    // 몇 명 인지
                    std::cout << num_bodies << " Bodies " << std::endl;
                    for (uint32_t i = 0; i < num_bodies; i++)
                    {
                        time_s = LocalMilli();
                        kLST kt; //임시 구조체 변수
                        k4abt_body_t body = body_frame.get_body(i);
                        for (int i = 0; i < (int)K4ABT_JOINT_COUNT; i++)
                        {
                            k4a_float3_t position = body.skeleton.joints[i].position;
                            k4a_quaternion_t orientation = body.skeleton.joints[i].orientation;
                            kt.id = frame_count;
                            kt.time_s = time_s;
                            kt.xp = to_string(position.v[0]);
                            kt.yp = to_string(position.v[1]);
                            kt.zp = to_string(position.v[2]);
                            kt.w = to_string(orientation.v[0]);
                            kt.x = to_string(orientation.v[1]);
                            kt.y = to_string(orientation.v[2]);
                            kt.z = to_string(orientation.v[3]);
                        }
                        klist.push_back(kt);
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
        std::cout << "Finished body tracking processing!" << std::endl;
    }
    catch (const std::exception& e)
    {
        // try-catch Error
        std::cerr << "Failed with exception:" << std::endl
            << "    " << e.what() << std::endl;
    }
#pragma endregion
    list<kLST>::iterator iter = klist.begin();
    while (iter != klist.end())//반복자가 리스트의 끝을 만날때까지 계속 반복한다.

    {
        kLST a = *iter;//반복자 변수 주소값을 넘긴다.
        cout << a.time_s << endl;
        cout << a.x + "\n" << a.y + "\n" << a.z + "\n" << a.w + "\n" << endl;//화면에 출력
        iter++;//반복자 주소값 쉬프트.

    }

}