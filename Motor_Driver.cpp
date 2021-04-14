#include <thread>
#include <opencv2/highgui/highgui_c.h>
#include "Display.h"
#include <chrono>
#include "Motor_Driver.h"
#include <arpa/inet.h>
using namespace std;
int Motor_Driver(float &positionX, float &positionY, float &RealAngle,float &targetAngle)
{

	int srv_sd, cli_sd, ret;
	if ((srv_sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	sockaddr_in svr_addr, cli_addr;
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons(7403);
	cli_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	std::cout << "start" << std::endl;
	while (1) 
	{
		//MTX.lock();
		//heading=RealAngle;
		//MTX.unlock();
		cv::waitKey(10);

		while (1) //until turn to target
		{

			float angleDist = RealAngle - targetAngle;
			if (angleDist > 180) angleDist =  angleDist-360;
			if (angleDist < -180) angleDist = 360 + angleDist;
			cout<<"  Motor_Driver--  heading "<<RealAngle<<" Target "<<targetAngle<<" DIST  "<<angleDist<<endl;

			if (angleDist > 4 || angleDist < -4)
			{
				if (angleDist < 0)
				{
					char SendBuf[15] ="right";
					sendto(sock_fd, SendBuf,5, 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));

				}
				else
				{
					char SendBuf[15] = "left ";

					sendto(sock_fd, SendBuf,5, 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));

				}
			}
			else
				break;
		}
		char SendBuf[15] = "front";
		sendto(sock_fd, SendBuf,5, 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));
	}
}
