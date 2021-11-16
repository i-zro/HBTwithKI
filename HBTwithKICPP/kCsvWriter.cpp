#define _CRT_SECURE_NO_WARNINGS
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
	struct tm* t;
	timer = time(NULL); // 1970년 1월 1일 0시 0분 0초부터 시작하여 현재까지의 초
	t = localtime(&timer); // 포맷팅을 위해 구조체에 넣기

	string whatTime = to_string(t->tm_year + 1900) + "년" +
		to_string(t->tm_mon + 1) + "월" +
		to_string(t->tm_mday) + "일" +
		to_string(t->tm_hour) + "시" +
		to_string(t->tm_min) + "분" +
		to_string(t->tm_sec) + "초"; //현재 시간 string 변수

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