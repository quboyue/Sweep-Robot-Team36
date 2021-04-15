#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <signal.h>
#include  "time.h"
#include "Display.h"
#include <thread>
#include <opencv2/opencv.hpp>

using namespace std;
int DFS_search(vector<vector<int>> &list_target, vector<vector<int>> &Map);
int DFS_loop(float &positionX, float &positionY, cv::Mat &pic, vector<vector<int>> &list_target);