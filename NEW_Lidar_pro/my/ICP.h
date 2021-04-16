#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <signal.h>
#include  "time.h"
#include "rplidar.h" //RPLIDAR standard sdk, all-in-one header
#include "kdtree.h"
#include "Display.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
using namespace std;
vector<vector<vector<float>>> ICP_D(vector<vector<float>> A, vector<vector<float>> B);
