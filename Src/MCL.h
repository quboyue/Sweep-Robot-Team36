#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <random> //C++ 11 Random Numbers
#include <fstream>
#include <opencv2/opencv.hpp>
using namespace std;
float  mod(float first_term, float second_term);
double gen_real_random();
vector<float> max(vector<float> Weight, int n);
int MCL_Main(float &startx, float &starty, vector<vector<float>> &nowset,float &positionX,float &positionY);