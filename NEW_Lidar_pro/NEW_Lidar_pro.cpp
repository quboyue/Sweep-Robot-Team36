
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <signal.h>
#include  "time.h"
#include "rplidar.h" //RPLIDAR standard sdk, all-in-one header
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <thread>

#include "kdtree.h"
#include "Display.h"
#include "ICP.h"
#include "MCL.h"
#include "DWA.h"
#include "UDP.h"
#include "DFS_Path.h"
#include "Motor_Driver.h"
#define PI acos(-1)

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

#ifdef _WIN32
#include <Windows.h>
#define delay(x)   ::Sleep(x)
#else
#include <unistd.h>
static inline void delay(_word_size_t ms) {
	while (ms >= 1000) {
		usleep(1000 * 1000);
		ms -= 1000;
	};
	if (ms != 0)
		usleep(ms * 1000);
}
#endif

using namespace rp::standalone::rplidar;
using namespace std;
bool checkRPLIDARHealth(RPlidarDriver * drv)
{
	u_result     op_result;
	rplidar_response_device_health_t healthinfo;


	op_result = drv->getHealth(healthinfo);
	if (IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
		cout << "RPLidar health status : \n" << healthinfo.status;
		if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
			fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
			// enable the following code if you want rplidar to be reboot by software
			// drv->reset();
			return false;
		}
		else {
			return true;
		}

	}
	else {
		fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
		return false;
	}
}
bool ctrl_c_pressed;
void ctrlc(int)
{
	ctrl_c_pressed = true;
}

int onFinished(RPlidarDriver *drv)
{

	RPlidarDriver::DisposeDriver(drv);
	drv = NULL;
	return 0;


}




int main()
{



	const char * opt_com_path = NULL;
	_u32         baudrateArray[2] = { 115200, 256000 };
	_u32         opt_com_baudrate = 0;
	u_result     op_result;

	bool useArgcBaudrate = false;

	("Ultra simple LIDAR data grabber for RPLIDAR.");

	// read serial port from the command line...
	// or set to a fixed value: e.g. "com3" 

	// read baud rate from the command line if specified...

	if (!opt_com_path) {
#ifdef _WIN32
		// use default com port
		opt_com_path = "\\\\.\\com5";
#elif __APPLE__
		opt_com_path = "/dev/tty.SLAB_USBtoUART";
#else
		opt_com_path = "/dev/ttyUSB0";
#endif
	}

	// create the driver instance
	RPlidarDriver * drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
	if (!drv) {
		fprintf(stderr, "insufficent memory, exit\n");
		exit(-2);
	}

	rplidar_response_device_info_t devinfo;
	bool connectSuccess = false;
	// make connection...
	if (useArgcBaudrate)
	{
		if (!drv)
			drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
		if (IS_OK(drv->connect(opt_com_path, opt_com_baudrate)))
		{
			op_result = drv->getDeviceInfo(devinfo);

			if (IS_OK(op_result))
			{
				connectSuccess = true;
			}
			else
			{
				delete drv;
				drv = NULL;
			}
		}
	}
	else
	{
		size_t baudRateArraySize = (sizeof(baudrateArray)) / (sizeof(baudrateArray[0]));
		for (size_t i = 0; i < baudRateArraySize; ++i)
		{
			if (!drv)
				drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
			if (IS_OK(drv->connect(opt_com_path, baudrateArray[i])))
			{
				op_result = drv->getDeviceInfo(devinfo);

				if (IS_OK(op_result))
				{
					connectSuccess = true;
					break;
				}
				else
				{
					delete drv;
					drv = NULL;
				}
			}
		}
	}
	if (!connectSuccess) {

		fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n"
			, opt_com_path);
		//goto on_finished;
		onFinished(drv);
	}

	// print out the device serial number, firmware and hardware version number..
	cout << "RPLIDAR S/N: ";
	for (int pos = 0; pos < 16; ++pos) {
		cout << devinfo.serialnum[pos] << endl;
	}
	
	printf("\n"
	"Firmware Ver: %d.%02d\n"
	"Hardware Rev: %d\n"
	, devinfo.firmware_version >> 8
	, devinfo.firmware_version & 0xFF
	, (int)devinfo.hardware_version);

	

	// check health...
	if (!checkRPLIDARHealth(drv)) {
		//goto on_finished;
		onFinished(drv);
	}

	signal(SIGINT, ctrlc);

	drv->startMotor();
	// start scan...
	drv->startScan(0, 1);

	// fetech result and print it out...
	ofstream OutFile("Test.txt");
	float Startx = 500;
	float Starty = 500;
	float heading = 0;
	float RealAngle=0;
	vector<vector<float>> nowset(2);
	vector<vector<float>> lastset(2);
	vector<vector<vector<float>>> Result;
	vector<vector<float>> R(2, vector<float>(2));
	vector<vector<float>> T(2, vector<float>(1));
	vector<vector<float>> A(2, vector<float>(3));
	vector<vector<float>> B(2, vector<float>(3));

	float positionX=500;
	float positionY=500;
	float targetAngle = 0;
	vector<vector<int>> list_target;

	vector<float> thetas;
	vector<float>  dists;
	clock_t start, finish;
	double  duration;
	int i = 0;
	bool MCL_ready = FALSE;
	cv::Mat pic(1000, 1000, CV_8UC1, cv::Scalar(round(255 * 0.5)));

	thread th0(UDP,ref(RealAngle));
	th0.detach();

	thread th2(MCL_Main, ref(nowset),ref(positionX),ref(positionY),ref(MCL_ready));
	th2.detach();

	int mode = 1;
	bool thread_start = 1;
	while (1)
	{
		

		if (MCL_ready && thread_start) 
		{

			if (mode == 1) //mode1 导航模式
			{
				pic = cv::imread("test2.png");
				vector<cv::Mat> channels;
				split(pic, channels);
				pic = channels.at(0);

				thread th1(DDA_line, ref(positionX), ref(positionY), ref(thetas), ref(dists), ref(pic), ref(MCL_ready));
				th1.detach();
			}
			else//建图模式
			{

				thread th1(DDA_line, ref(Startx), ref(Starty), ref(thetas), ref(dists), ref(pic), ref(MCL_ready));
				th1.detach();//导航模式positionX //建图模式Startx
			}

			//vector<vector<int>> target(2, vector<int>(1));
			//target[0][0] = 600;
			//target[1][0] = 600;

			thread th3(DFS_loop, ref(positionX), ref(positionY), ref(pic), ref(list_target));
			th3.detach();
			//list_target[0][0] = 600;
			//list_target[0][1] = 600;
			thread th4(DWA_MAIN_loop, ref(nowset), ref(positionX), ref(positionY), ref(list_target), ref(RealAngle), ref(targetAngle));
			th4.detach();
			//thread th5(Motor_Driver, ref(positionX), ref(positionY), ref(RealAngle), ref(targetAngle));
			//th5.detach();
			thread_start = 0;
		
		}
		clock_t start, finish;
		start = clock();
		rplidar_response_measurement_node_t nodes[8192];
		size_t   count = _countof(nodes);
		op_result = drv->grabScanData(nodes, count);
		MTX.lock();
		if (IS_OK(op_result))
		{
			drv->ascendScanData(nodes, count);
			for (int pos = 0; pos < (int)count; ++pos)
			{
	
				float theta = (float(nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) / 64.0f);
				float dist = float(nodes[pos].distance_q2 / 4.0f);
				if (dist != 0 && dist<3500)
				{
					theta = theta + RealAngle;
					while (theta > 360) theta-= 360;
					while (theta < 360) theta += 360;
					thetas.push_back(theta);
					dists.push_back(dist / 10);
					nowset[0].push_back(sin(theta*PI / 180.0)*dist / 10);
					nowset[1].push_back(cos(theta*PI / 180.0)*dist / 10);
				}

			}

		}

		MTX.unlock();

		if (ctrl_c_pressed) {
			cout << "End" << endl;
			system("pasue");
			return 1;
			break;

		}

		if (lastset[0].size())
		{
			if (mode == 2) 
			{
				cout << "ICP" << endl;
				Result = ICP_D(lastset, nowset);
				R = Result[0];
				T = Result[1];
				cout << R[0][0] << "  " << R[0][1] << endl;
				cout << R[1][0] << "  " << R[1][1] << endl;
				cout << T[0][0] << "  " << T[1][0] << endl;
				Startx += T[0][0] * 2;
				Starty += T[1][0] * 2;
				cout << "POSITION   " << Startx << "  " << Starty << endl;
			}
			if (mode == 1)
				cv::waitKey(150);
				//给其他线程一些时间来读取


		}


		MTX.lock();
		//printf("i:%d\n", i);
		lastset[0].clear();
		lastset[1].clear();
		lastset = nowset;
		//printf("size lastset%d\n", lastset.size());
		//printf("size lastset%d\n---------\n", lastset[0].size());
		nowset[0].clear();
		nowset[1].clear();

		dists.clear();
		thetas.clear();
		MTX.unlock();
		i++;
		float duration = 0;
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//cout << "\n>>>>>>>>>>>>>>>>>>> ----seconds " << duration << endl;
	}


	drv->stop();
	drv->stopMotor();
	// done!




}
