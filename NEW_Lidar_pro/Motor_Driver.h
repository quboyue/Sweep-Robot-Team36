#pragma once
#pragma once
#include <winsock2.h>	//delte on LINUX��������
#include <WS2tcpip.h>	//delte on LINUX��������
#include <iostream>
#include <string>
#include <math.h>
#include <vector>


int Motor_Driver(float &positionX, float &positionY, float &heading,  float &targetAngle);