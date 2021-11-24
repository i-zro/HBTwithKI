#ifndef __Forsync
#define __Forsync

#include <assert.h>
#include <future>
#include <k4a/k4a.hpp>
#include <k4abt.hpp>

#include <list>

#include <thread>
#include "kPrinter.h"
#include "iPrinter.h"
#include "SerialPort.h"
#include <KITOOLs.h>

using namespace std;
using std::thread;

void toKLST(k4abt_body_t body);
void toILST();
void syncFunc();
void parallel();
#endif