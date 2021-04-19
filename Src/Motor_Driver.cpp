#include <thread>
#include <opencv2/highgui/highgui_c.h>
#include "Display.h"
#include <chrono>
#include "Motor_Driver.h"
#include <arpa/inet.h>
#include <string>
#include <fstream>
using namespace std;
int Motor_Driver(float &positionX, float &positionY, float &RealAngle,float &targetAngle,vector<vector<int>> &list_target,string &command)
{
	cv::waitKey(3000);
	ofstream out_dfs("DFS.txt");
	for(int i=0;i<list_target.size();i++)
	{
	out_dfs<<list_target[i][0]<<" "<<list_target[i][1]<<endl;

	}
	out_dfs.close();

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

	svr_addr.sin_family = AF_INET;
        svr_addr.sin_port = htons(7403);
        svr_addr.sin_addr.s_addr = inet_addr("192.168.0.105");//my pc

	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	int sock_fd2=socket(AF_INET, SOCK_DGRAM, 0);

	std::cout << "start" << std::endl;
        std::string send_data="";
	char pc_buff[512];

	while (1)
	{

		while (1) //until turn to target
		{
//			MTX.lock();
			send_data="Heading:"+to_string(RealAngle)+" TargetAngle:"+to_string(targetAngle)+" PositionX:"+to_string(positionX)+" PositionY:"+to_string(positionY)+" TargetX:"+to_string(list_target[0][0])+" TargetY:"+to_string(list_target[0][1])+" ";
			//for(int i=0;i<list_target.size();i++)
			//	send_data+=to_string(list_target[i][0])+":"+to_string(list_target[i][1])+"-";
			send_data+=" ";
			send_data+=to_string(list_target.size());
			send_data+=" ";
//			MTX.unlock();
        	        send_data.copy(pc_buff,send_data.size(),0);
	                sendto(sock_fd2, pc_buff,512, 0, (struct sockaddr *)&svr_addr, sizeof(svr_addr));
			cout<<" Motor Driver-- send"<<endl;
			cv::waitKey(50);
			float angleDist = RealAngle - targetAngle;
			if (angleDist > 180) angleDist =  angleDist-360;
			if (angleDist < -180) angleDist = 360 + angleDist;
//			cout<<"  Motor_Driver--  heading "<<RealAngle<<" Target "<<targetAngle<<" DIST  "<<angleDist<<endl;

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
