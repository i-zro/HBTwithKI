#ifndef KI_TOOLS
#define KI_TOOLS
#include <assert.h>
#include <iostream>
#include <string>
#include <time.h>
#include <locale>
#include <sys/timeb.h>
#include <sstream>  
using namespace std;
using std::string;
//IMU 구조체
struct iLST {
	int time_s;
	std::string id = "";
	std::string x = "";
	std::string y = "";
	std::string z = "";
	std::string w = "";
	std::string b = ""; //배터리 잔량
};

//Kinect 구조체
struct kLST {
	int time_s;
	int id;
	std::string xp = "";
	std::string yp = "";
	std::string zp = "";
	std::string x = "";
	std::string y = "";
	std::string z = "";
	std::string w = "";
};

//class kCls
//{
//private:
//	int time_s;
//	int id;
//	string xp;
//	string yp;
//	string zp;
//	string x;
//	string y;
//	string z;
//	string w;
//public:
//	kCls(int time_s, int id, string xp, string yp, string zp, string x, string y, string z, string w)
//	{
//		time_s = time_s;
//		id = id;
//		string xp = xp;
//		string yp = yp;
//		string zp = zp;
//		string x = x;
//		string y = y;
//		string z = z;
//		string w = w;
//	}
//
//	kCls()
//	{
//		time_s = 0;
//		id = 0;
//		xp = "";
//		yp = "";
//		zp = "";
//		x = "";
//		y = "";
//		z = "";
//		w = "";
//	}
//
//	void SetkClsInfo(int time_s, int id, string xp, string yp, string zp, string x, string y, string z, string w)
//	{
//		time_s = time_s;
//		id = id;
//		xp = xp;
//		yp = yp;
//		zp = zp;
//		x = x;
//		y = y;
//		z = z;
//		w = w;
//	}

//	void ShowkClsInfo() const
//	{
//		cout << "time: " << time_s << endl;
//		cout << "xp: " << xp << endl;
//		cout << "yp: " << yp << endl;
//		cout << "zp: " << zp << endl;
//		cout << "x: " << x << endl;
//		cout << "y: " << y << endl;
//		cout << "z: " << z << endl;
//		cout << "w: " << w << endl;
//	}
//
//	~kCls()
//	{
//	}
//};

std::string saveCsv(int which);
int LocalMilli();
#endif