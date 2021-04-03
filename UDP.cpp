#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
using namespace  std;

//添加动态库的lib
#pragma comment(lib, "ws2_32.lib")
//UDP客户端
int  UDP(float &RealAngle) {

	//初始化winsock2环境
	WSADATA  wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		cout << "WSAStartup error:" << GetLastError() << endl;
		return 0;
	}

	int srv_sd, cli_sd, ret;


	if ((srv_sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	sockaddr_in svr_addr, cli_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(7402);
	svr_addr.sin_addr.s_addr = 0;
	if ((ret = bind(srv_sd, (struct sockaddr*)&svr_addr, addrlen)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	int inital = 0;
	float AngleInital = 0;

	while (1)
	{
		char buf[1024] = {};
		ret = recvfrom(srv_sd, buf, 1024, 0, (struct sockaddr*)&cli_addr, &addrlen);
		if (ret > 0)
		{
			string bufstring;
			bufstring = buf;
			float AngleInput = stof(bufstring);
			if (inital < 10)
			{
				inital += 1;
				AngleInital += AngleInput;
			}
			else
			{

				RealAngle = AngleInput - (AngleInital / inital);
				//cout << RealAngle << endl;
			}
		}
	}
	return 0;

}