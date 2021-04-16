
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <signal.h>
#include  "time.h"
#include "rplidar.h" //RPLIDAR standard sdk, all-in-one header
#include "kdtree.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
using namespace std;
using namespace cv;
std::mutex MTX;
#define PI acos(-1)

void DDA_line(float &Startx, float &Starty, float &heading, vector<float> &thetas, vector<float> &dists);
