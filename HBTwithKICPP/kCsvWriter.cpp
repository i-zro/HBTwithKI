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
	// ���� �̸� ������
	time_t timer;
	struct tm* t;
	timer = time(NULL); // 1970�� 1�� 1�� 0�� 0�� 0�ʺ��� �����Ͽ� ��������� ��
	t = localtime(&timer); // �������� ���� ����ü�� �ֱ�

	string whatTime = to_string(t->tm_year + 1900) + "��" +
		to_string(t->tm_mon + 1) + "��" +
		to_string(t->tm_mday) + "��" +
		to_string(t->tm_hour) + "��" +
		to_string(t->tm_min) + "��" +
		to_string(t->tm_sec) + "��"; //���� �ð� string ����

	csvName = "KinData/" + whatTime + ".csv";
	cout << "file name : " + csvName << endl;
	//���� �̸� ���ϱ⸸ �ϴ� �Լ�
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