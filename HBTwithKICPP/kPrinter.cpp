#include <assert.h>
#include <k4a/k4a.hpp>
#include <k4abt.hpp>

#include<iostream>
#include<cstring>
#include "kPrinter.h"

using namespace std;

string printJoint(int num)
{
    string joint; // 관절 좌표 이름
    if (num == 0) { joint = "PELVIS"; }
    else if (num == 1) { joint = "SPINE_NAVEL"; }
    else if (num == 2) { joint = "SPINE_CHEST"; }
    else if (num == 3) { joint = "NECK"; }
    else if (num == 4) { joint = "CLAVICLE_LEFT"; }
    else if (num == 5) { joint = "SHOULDER_LEFT"; }
    else if (num == 6) { joint = "ELBOW_LEFT"; }
    else if (num == 7) { joint = "WRIST_LEFT"; }
    else if (num == 8) { joint = "HAND_LEFT"; }
    else if (num == 9) { joint = "HANDTIP_LEFT"; }
    else if (num == 10) { joint = "THUMB_LEFT"; }
    else if (num == 11) { joint = "CLAVICLE_RIGHT"; }
    else if (num == 12) { joint = "SHOULDER_RIGHT"; }
    else if (num == 13) { joint = "ELBOW_RIGHT"; }
    else if (num == 14) { joint = "WRIST_RIGHT"; }
    else if (num == 15) { joint = "HAND_RIGHT"; }
    else if (num == 16) { joint = "HANDTIP_RIGHT"; }
    else if (num == 17) { joint = "THUMB_RIGHT"; }
    else if (num == 18) { joint = "HIP_LEFT"; }
    else if (num == 19) { joint = "KNEE_LEFT"; }
    else if (num == 20) { joint = "ANKLE_LEFT"; }
    else if (num == 21) { joint = "FOOT_LEFT"; }
    else if (num == 22) { joint = "HIP_RIGHT"; }
    else if (num == 23) { joint = "KNEE_RIGHT"; }
    else if (num == 24) { joint = "ANKLE_RIGHT"; }
    else if (num == 25) { joint = "FOOT_RIGHT"; }
    else if (num == 26) { joint = "HEAD"; }
    else if (num == 27) { joint = "NOSE"; }
    else if (num == 28) { joint = "EYE_LEFT"; }
    else if (num == 29) { joint = "EAR_LEFT"; }
    else if (num == 30) { joint = "EYE_RIGHT"; }
    else if (num == 31) { joint = "EAR_RIGHT"; }
    return joint;
}

void print_body_information(k4abt_body_t body)
{
    std::cout << "Body ID: " << body.id << std::endl;
    for (int i = 0; i < (int)K4ABT_JOINT_COUNT; i++)
    {
        k4a_float3_t position = body.skeleton.joints[i].position;
        k4a_quaternion_t orientation = body.skeleton.joints[i].orientation;
        cout << printJoint(i);
        printf(" : Position[mm] ( %f, %f, %f ); Orientation ( %f, %f, %f, %f); \n",
            position.v[0], position.v[1], position.v[2], orientation.v[0], orientation.v[1], orientation.v[2], orientation.v[3]);
    }
}





