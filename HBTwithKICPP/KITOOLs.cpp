#include "KITOOLs.h"

using namespace std;

string csvName;
const char* kinDir = "KinData";

void saveCsv(int which) {
    if(which==1){
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
    else if (which == 2) {
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

        csvName = "ImuData/" + whatTime + ".csv";
        cout << "file name : " + csvName << endl;
        //파일 이름 정하기만 하는 함수
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
        to_string(t.tm_sec); //현재 시간 string 변수

    long long time_last;
    time_last = time(NULL);

    //밀리초 위한 ftimer
    struct timeb t1;
    ftime(&t1);
    time_t ttt = t1.millitm;
    std::stringstream ss;
    ss << ttt;
    std::string ts = wTime + ss.str();
    int nts = stoi(ts);
    return nts;
}

