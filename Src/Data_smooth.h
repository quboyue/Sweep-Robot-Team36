#pragma once
#include<iostream>
#include <string>
#include <math.h>
#include <vector>

float float_smooth(std::vector<float> &smooth_list, float &new_item, int length);
float int_smooth(std::vector<int> &smooth_list, int &new_item, int length);
