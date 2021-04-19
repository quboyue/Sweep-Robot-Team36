
#include <iostream>
#include <thread>
#include <wiringPi.h>
#include <opencv2/opencv.hpp>

#include <netinet/in.h>
#include <unistd.h>
std::mutex MTX;

using namespace std;
//stand alone

int GPIO_loop(string &command)
{
	string last_command;
	while (1)
	{

		MTX.lock();
		if (last_command != command)
		{
			cout << command << endl;
			if (command == "front")
			{
				digitalWrite(5, HIGH);
				digitalWrite(7, LOW);
				cv::waitKey(2);
			}
			if (command == "right")
			{
				digitalWrite(5, HIGH);
				digitalWrite(7, HIGH);
				cv::waitKey(2);
			}
			if (command == "left ")
			{
				digitalWrite(5, LOW);
				digitalWrite(7, LOW);
				cv::waitKey(2);
			}

			last_command = command;
		}
		MTX.unlock();

		digitalWrite(4, HIGH);
		digitalWrite(2, HIGH);
		cv::waitKey(2);
		digitalWrite(4, LOW);
		digitalWrite(2, LOW);
		cv::waitKey(2);

	}

}


int  main() {

	cv::waitKey(1000);
	cout << "GPIO start" << endl;
	if (-1 == wiringPiSetup())
	{
		cout << "setup error\n";
		return 0;
	}
	pinMode(4, OUTPUT);//16
	pinMode(5, OUTPUT);//18

	pinMode(2, OUTPUT); // 13
	pinMode(7, OUTPUT);//7

	int srv_sd, ret;
	if ((srv_sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	sockaddr_in svr_addr, cli_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(7403);
	svr_addr.sin_addr.s_addr = 0;

	if ((ret = bind(srv_sd, (struct sockaddr*)&svr_addr, addrlen)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}


	string command;
	thread th1(GPIO_loop,ref(command));
	th1.detach();

	
	while (1)
	{
		char buf[15] = {};
		ret = recvfrom(srv_sd, buf, 1024, 0, (struct sockaddr*)&cli_addr, &addrlen);
		if (ret > 0)
		{
				MTX.lock();
				command = buf;
				MTX.unlock();
		}

	}
	return 0;

}
