#include "SerialPort.h"
#include "iPrinter.h"
#include "KITOOLs.h"
#include "CsvWriter.hpp"

using namespace std;
using std::string;
using namespace csv;

//뒤에 있는 개행 문자 제거
static inline std::string& rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

vector<string> split(string s, string divid) {
	vector<string> v;
	char* c = strtok((char*)s.c_str(), divid.c_str());
	while (c) {
		v.push_back(c);
		c = strtok(NULL, divid.c_str());
	}
	return v;
}

string iStrR()
{
	CSerialPort serialComm;
	serialComm.OpenPort(L"COM3");
	serialComm.ConfigurePort(921600, 8, FALSE, NOPARITY, ONESTOPBIT);
	char temp;
	string s1 = "";
	int num = 0;

	while (1) {
		temp = serialComm.ReadChar();
		s1 += temp;
		if (temp == '\n')
		{
			num += 1;
			//cout << "엔터 발견, 그만!"<<endl;
			
		}
		if (num == 1) {
			break;
		}
	}

	serialComm.ClosePort();
	return rtrim(s1);
}



void itoL()
{
	list<iLST> ilist; // imu 값 담을 리스트
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
			iLST it; //임시 구조체 변수
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
			//cout << "엔터 발견, 그만!"<<endl;

		}
		if (num == 200) {
			break;
		}
	}

	serialComm.ClosePort();
	list<iLST>::iterator iter = ilist.begin();
	while (iter != ilist.end())//반복자가 리스트의 끝을 만날때까지 계속 반복한다.

	{
		iLST a = *iter;//반복자 변수 주소값을 넘긴다.
		cout << a.time_s << endl;
		cout << a.id + "\n" << a.x + "\n" << a.y + "\n" << a.z + "\n" << a.w + "\n" << endl;//화면에 출력
		iter++;//반복자 주소값 쉬프트.

	}
}

void iTest() {
	for(int i=0;i<100;i++){
		cout<<iStrR()<<endl;
	}
}

int iBuff()
{
	CSerialPort serialComm;
	serialComm.OpenPort(L"COM3");
	serialComm.ConfigurePort(921600, 8, FALSE, NOPARITY, ONESTOPBIT);


	std::string sent1;
	byte buff;
	while (1) {
		serialComm.ReadByte(buff);
		cout << buff;
	}

	return 0;
}

void iChar()
{
	CSerialPort serialComm;
	serialComm.OpenPort(L"COM3");
	serialComm.ConfigurePort(921600, 8, FALSE, NOPARITY, ONESTOPBIT);

	while (1) {
		cout << serialComm.ReadChar();
	}
}

void iStr()
{
	CSerialPort serialComm;
	serialComm.OpenPort(L"COM3");
	serialComm.ConfigurePort(921600, 8, FALSE, NOPARITY, ONESTOPBIT);
	char temp;
	string s1="";
	while (1) {
		temp = serialComm.ReadChar();
		s1 += temp;
		if (temp == '\n')
		{
			//cout << "엔터 발견, 그만!"<<endl;
			break;
		}
	}
	cout << "딱 하나만 출력 : " + s1;
}

