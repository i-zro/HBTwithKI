#include "KITOOLs.h"

using namespace std;

string csvName;
const char* kinDir = "KinData";

void saveCsv(int which) {
    if(which==1){
    // ���� �̸� ������
    time_t timer;
    timer = time(NULL);

    struct tm t;
    localtime_s(&t, &timer);

    string whatTime = to_string(t.tm_year + 1900) + "��" +
        to_string(t.tm_mon + 1) + "��" +
        to_string(t.tm_mday) + "��" +
        to_string(t.tm_hour) + "��" +
        to_string(t.tm_min) + "��" +
        to_string(t.tm_sec) + "��"; //���� �ð� string ����

    csvName = "KinData/" + whatTime + ".csv";
    cout << "file name : " + csvName << endl;
    //���� �̸� ���ϱ⸸ �ϴ� �Լ�
    }
    else if (which == 2) {
        // ���� �̸� ������
        time_t timer;
        timer = time(NULL);

        struct tm t;
        localtime_s(&t, &timer);

        string whatTime = to_string(t.tm_year + 1900) + "��" +
            to_string(t.tm_mon + 1) + "��" +
            to_string(t.tm_mday) + "��" +
            to_string(t.tm_hour) + "��" +
            to_string(t.tm_min) + "��" +
            to_string(t.tm_sec) + "��"; //���� �ð� string ����

        csvName = "ImuData/" + whatTime + ".csv";
        cout << "file name : " + csvName << endl;
        //���� �̸� ���ϱ⸸ �ϴ� �Լ�
    }
}

int LocalMilli() {
    //localtimer
    time_t timer;
    timer = time(NULL);

    struct tm t;
    localtime_s(&t, &timer);

    string wTime = to_string(t.tm_hour) +
        to_string(t.tm_min) +
        to_string(t.tm_sec); //���� �ð� string ����

    long long time_last;
    time_last = time(NULL);

    //�и��� ���� ftimer
    struct timeb t1;
    ftime(&t1);
    time_t ttt = t1.millitm;
    std::stringstream ss;
    ss << ttt;
    std::string ts = wTime + ss.str();
    int nts = stoi(ts);
    return nts;
}

