#include<iostream>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <random> //C++ 11 Random Numbers
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "Display.h"
using namespace std;


int DWA(vector<vector<int>> &target, vector<vector<float>> &nowset, float &positionX, float &positionY, cv::Mat &pic, float &RealAngle,float &targetAngle, float &dist2target);
int DWA_MAIN_loop(vector<vector<float>> &nowset, float &positionX, float &positionY, vector<vector<int>> &target, float &RealAngle, float &targetAngle);


