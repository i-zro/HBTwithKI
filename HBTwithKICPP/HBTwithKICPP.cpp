#include <k4a/k4a.hpp>
#include <k4abt.hpp>
#include <iostream>
#include <cstring>
#include "kPrinter.h"

using namespace std;

int main() {

    int num;
    cout << "* * * * * * * * * * * * * * * * * * \n" <<
        "Enter an integer from 1 to 5 : \n" <<
        "(1) : Print your body for 100 frames \n" <<
        "(2) : Save it to csv file \n " <<
        "> ";
    cin >> num;

    if (num == 1) {
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
    else if (num == 2) {
        cout << "This number is 2" << endl;
    }
    else if (num == 3) {
        cout << "This number is 3" << endl;
    }
    else if (num == 4) {
        cout << "This number is 4" << endl;
    }
    else if (num == 5) {
        cout << "This number is 5" << endl;
    }
    else {
        cout << "Not a number of ranges" << endl;
    }
    return 0;
}