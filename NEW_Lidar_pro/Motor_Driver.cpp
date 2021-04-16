#pragma once
#include <winsock2.h>	//delte on LINUX£¡£¡£¡£¡
#include <WS2tcpip.h>	//delte on LINUX£¡£¡£¡£¡
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;
int Motor_Driver(float &positionX, float &positionY, float &heading,float &targetAngle)
{
	//delte on LINUX£¡£¡£¡£¡
	WSADATA  wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		cout << "WSAStartup error:" << GetLastError() << endl;
		return 0;
	}//delte on LINUX£¡£¡£¡£¡
	int srv_sd, cli_sd, ret;
	if ((srv_sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	sockaddr_in svr_addr, cli_addr;
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons(7403);
	cli_addr.sin_addr.s_addr = inet_addr("192.168.0.108");
	SOCKET SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	std::cout << "start" << std::endl;
	while (1) 
	{
		while (1) //until turn to target
		{
			float angleDist = heading - targetAngle;
			if (angleDist > 180) angleDist =  angleDist-360;
			if (angleDist < -180) angleDist = 360 + angleDist;
			//cout << " angleDist  "   << angleDist << endl;
			if (angleDist > 3 || angleDist < -3)
			{
				if (angleDist < 0) 
				{
					char SendBuf[1024] = "right";
					sendto(SendSocket, SendBuf, 5, 0, (SOCKADDR *)&cli_addr, sizeof(cli_addr));
				}
				else
				{
					char SendBuf[1024] = "left ";
					sendto(SendSocket, SendBuf, 5, 0, (SOCKADDR *)&cli_addr, sizeof(cli_addr));
				}
			}
			else
				break;
		}
		char SendBuf[1024] = "front";
		sendto(SendSocket, SendBuf, 5, 0, (SOCKADDR *)&cli_addr, sizeof(cli_addr));
	}
}