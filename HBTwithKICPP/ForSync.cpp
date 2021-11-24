#include <ForSync.h>

list<kLST> kli;
list<iLST> ili;
k4abt_body_t body;

void syncFunc() {
    
    try
    {
        // * common code
        // kinectDEF : kinect color ī�޶� �� depth ī�޶�, tracker���� ����, device open(kinect �����볪 �� ��°�ĵ� �޶��� �� ����) ����
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
                

                // frame ó�� ����

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
                    // �� �� ����
                    std::cout << num_bodies << " Bodies " << std::endl;
                    for (uint32_t i = 0; i < num_bodies; i++)
                    {
                        body = body_frame.get_body(i);
                        
                        auto f1 = std::async([]() {toKLST(body);});
                        auto f2 = std::async([]() {toILST();});

                        f1.get();
                        f2.get();
                    }
                }
                // * common code
                // Error ó��
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

}

void toKLST(k4abt_body_t body) {
    int time_s = LocalMilli();
    kLST kt; //�ӽ� ����ü ����
    for (int i = 0; i < (int)K4ABT_JOINT_COUNT; i++)
    {
        k4a_float3_t position = body.skeleton.joints[i].position;
        k4a_quaternion_t orientation = body.skeleton.joints[i].orientation;
        kt.time_s = time_s;
        kt.xp = to_string(position.v[0]);
        kt.yp = to_string(position.v[1]);
        kt.zp = to_string(position.v[2]);
        kt.w = to_string(orientation.v[0]);
        kt.x = to_string(orientation.v[1]);
        kt.y = to_string(orientation.v[2]);
        kt.z = to_string(orientation.v[3]);
        kli.push_back(kt);
    }
}

void printKLST() {
    list<kLST>::iterator iter = kli.begin();
    while (iter != kli.end())//�ݺ��ڰ� ����Ʈ�� ���� ���������� ��� �ݺ��Ѵ�.

    {
        kLST a = *iter;//�ݺ��� ���� �ּҰ��� �ѱ��.
        cout << a.time_s << endl;
        cout << a.x + "\n" << a.y + "\n" << a.z + "\n" << a.w + "\n" << endl;//ȭ�鿡 ���
        iter++;//�ݺ��� �ּҰ� ����Ʈ.

    }
}


void toILST() {
    list<iLST> ilist; // imu �� ���� ����Ʈ
    CSerialPort serialComm;
    serialComm.OpenPort(L"COM3");
    serialComm.ConfigurePort(921600, 8, FALSE, NOPARITY, ONESTOPBIT);
    char temp;
    string s1 = "";
    int num = 0;

    while (1) {
        temp = serialComm.ReadChar();
        s1 += temp;
        std::string delimiter = ",";
        if (temp == '\n')
        {
            size_t pos = 0;
            std::string token;
            iLST it; //�ӽ� ����ü ����
            vector<string> words = split(s1, delimiter);

            //for (const auto& str : words) {
            //	cout << str << endl;
            //}
            it.time_s = LocalMilli();
            it.id = words[0];
            it.x = words[1];
            it.y = words[2];
            it.z = words[3];
            it.w = words[4];
            //it.b = rtrim(words[5]);
            ilist.push_back(it);
            num += 1;
            //cout << "���� �߰�, �׸�!"<<endl;

        }
        if (num == 200) {
            break;
        }
    }

    serialComm.ClosePort();
}

void printiLST() {
    list<iLST>::iterator iter = ili.begin();
    while (iter != ili.end())//�ݺ��ڰ� ����Ʈ�� ���� ���������� ��� �ݺ��Ѵ�.

    {
        iLST a = *iter;//�ݺ��� ���� �ּҰ��� �ѱ��.
        cout << a.time_s << endl;
        cout << a.id + "\n" << a.x + "\n" << a.y + "\n" << a.z + "\n" << a.w + "\n" << endl;//ȭ�鿡 ���
        iter++;//�ݺ��� �ּҰ� ����Ʈ.

    }
}

void parallel() {
    syncFunc();
    auto f1 = std::async([]() { printKLST(); });
    auto f2 = std::async([]() { printiLST(); });
    f1.get();
    f2.get();
}