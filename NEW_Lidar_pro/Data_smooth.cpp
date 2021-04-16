
#include "Data_smooth.h"
float float_smooth(std::vector<float> &smooth_list,float &new_item,int length)
{
	float sum=0;
	smooth_list.push_back(new_item);
	if (smooth_list.size() > length) 
	{
		std::vector<float> ::iterator itor;
		itor = smooth_list.begin();
		smooth_list.erase(itor);
	}
	for (int i = 0; i < smooth_list.size(); i++) 
	{
		sum += smooth_list[i];
	}

	return sum/smooth_list.size();
}

float int_smooth(std::vector<int> &smooth_list, int &new_item,int length)
{
	float sum=0;
	smooth_list.push_back(new_item);
	if (smooth_list.size() > length)
	{
		std::vector<int> ::iterator itor;
		itor = smooth_list.begin();
		smooth_list.erase(itor);
	}
	for (int i = 0; i < smooth_list.size(); i++)
	{
		sum += smooth_list[i];
	}

	return sum/ smooth_list.size();
}