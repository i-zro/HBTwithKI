#include <iostream>
#include <stdio.h>
#include <string>
#include "SerialPort.h"

using namespace std;

int iParse()
{
	CSerialPort serialComm;

	serialComm.OpenPort(L"COM3");
	serialComm.ConfigurePort(921600, 8, FALSE, NOPARITY, ONESTOPBIT);

	byte buff;
	while (1) {
		serialComm.ReadByte(buff);
		cout << buff;
	}
	return 0;
}

void ibasicPrint()
{
	iParse();
}