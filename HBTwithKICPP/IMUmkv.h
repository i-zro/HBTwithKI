#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <k4a/k4a.h>
#include <k4arecord/playback.h>
#include <k4abt.h>
#include <nlohmann/json.hpp>
#include <winnt.h>
#include <process.h>
#include <WinBase.h>

#include <BodyTrackingHelpers.h>
#include <Utilities.h>

using namespace std;
using namespace nlohmann;

// global
int frame_count = 0;
json frames_json = json::array();

bool doneGenerating;

//declare
bool check_depth_image_exists(k4a_capture_t capture);

typedef struct
{
    k4a_playback_t playback_handle;
    k4abt_tracker_t tracker;
} ThreadArgs;

unsigned __stdcall generate_body_frame(void* args);
unsigned __stdcall process_body_frame(void* args);
bool process_mkv(const char* input_path, const char* output_path);