#pragma once
//##include <winsock2.h>	//delte on LINUX£¡£¡£¡£¡
//#include <WS2tcpip.h>	//delte on LINUX£¡£¡£¡£¡

#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <math.h>
#include <vector>


int Motor_Driver(float &positionX, float &positionY, float &heading,  float &targetAngle,vector<vector<int>> &list_target,string &command);
