#include <assert.h>
#include <iostream>
#include <string>
#include <time.h>
#include <sys/timeb.h>
#include <sstream>  
#include "KITOOLs.h"

using namespace std;

void LocalMille() {
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
    std::cout << ts << std::endl;
}