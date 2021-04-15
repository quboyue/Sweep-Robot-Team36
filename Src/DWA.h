#include<iostream>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <random> //C++ 11 Random Numbers
#include <fstream>
#include <opencv2/opencv.hpp>
#include "Display.h"
using namespace std;


int DWA(vector<vector<float>> &target, vector<float> &dists, vector<float> &thetas, float &positionX, float &positionY, cv::Mat &pic);
int DWA_MAIN_loop(vector<float> &thetas, vector<float> &dists, float &positionX, float &positionY, vector<vector<float>> &target);

