#pragma once 
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <signal.h>
#include  "time.h"
#include "kdtree.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>

using namespace std;

extern std::mutex MTX;
#define PI acos(-1)

void DDA_line(float &Startx, float &Starty, float &heading, vector<float> &thetas, vector<float> &dists,cv::Mat &pic);
