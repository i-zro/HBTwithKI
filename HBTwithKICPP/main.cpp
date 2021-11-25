#include <time.h>
#include <future>
#include <list>
#include <stdio.h>
#include <sys/timeb.h> 
#include <k4a/k4a.hpp>
#include <k4abt.hpp>
#include <iostream>
#include <sstream>   
#include <cstring>
#include <thread>
#include <mutex>
#include "kPrinter.h"
#include "iPrinter.h"
#include "kCsvWriter.h"
#include "CsvWriter.hpp"
#include "KITOOLs.h"
//#include "ForSync.h";

using namespace std;
using namespace csv;
using std::thread;

int main() {
    int num;
    cout << "* * * * * * * * * * * * * * * * * * \n" <<
        "Enter an integer from 1 to 10 : \n" <<
        "(1) : K - Print your body for 100 frames (pos + quaternion) \n" <<
        "(2) : K - Save them as a csv file with 32 columns (pos + quaternion) \n " <<
        "(3) : K & I - Print your body for 100 frames with timestamp (pos + quaternion) \n " <<
        "(4) : IMU 출력 1 \n " <<
        "(5) : IMU 출력 2 (char return) \n " <<
        "(6) : IMU 출력 딱 한 줄만 \n " <<
        "(7) : IMU 출력 딱 한 줄만 string 반환\n " <<
        "(10) : Thread Test\n " <<
        "(11) : IMU 저장\n " <<
        "> ";
    cin >> num;

    if (num == 1) {
        kPrintF();
    }

    else if (num == 2) {
        kSaveF();
    }
    else if (num == 3)       {
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
                        cout<<LocalMilli();
                        print_body_information(body);
                        cout<<iBuff();
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
    else if (num == 4) {
        cout << iBuff();
    }
    else if (num == 5) {
        iChar();
    }
    else if (num == 6) {
        iStr();
    }
    else if (num == 7) {
        cout<<iStrR();
    }
    else if (num == 8) {
        itoL();
    }
    else if (num == 9) {
    thread t1(ktoL);
    thread t2(itoL);
    t1.join();
    t2.join();
    }
    else if (num == 10) {
    kSaveT();
    }
    else if (num == 11) {
    save_imu();
    }
    else if (num == 12) {
    thread t1(kSaveT);
    thread t2(save_imu);
    t1.join();
    t2.join();
    }
    else {
        cout << "Not a number of ranges" << endl;
    }
    return 0;
}