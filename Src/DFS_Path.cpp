#include "DFS_Path.h"
#include <opencv2/highgui/highgui_c.h>
using namespace std;

int DFS_search(vector<vector<int>> &list_target, vector<vector<int>> &Map);
int DFS_loop(float &positionX, float &positionY,cv::Mat &pic, vector<vector<int>> &list_target)
{
//	cv::namedWindow("DFS", CV_WINDOW_NORMAL);
//	cv::resizeWindow("DFS", 450, 450);
//	cv::moveWindow("DFS", 0, 450);

	cv::Mat local_map;
	vector<vector<int>> Map(50, vector<int>(50));
	cv::Mat show(cv::Size(100, 100), CV_8UC3);
	vector<vector<int>> list_accessilbe;

	int startx;
	int starty;
	int blur_size = 20;

//	cout<<" here 1"<<endl;
	while (1)
	{
		list_accessilbe.clear();

		MTX.lock();

		startx = positionX/blur_size;
		starty = positionY/blur_size;
		local_map = pic.clone();

		MTX.unlock();

		cv::blur(local_map, local_map, cv::Size(blur_size, blur_size), cv::Point(-1, -1));
		cv::resize(local_map, local_map, cv::Size(1000 / blur_size, 1000 / blur_size));
		cv::Rect ccomp;
		cv::floodFill(local_map, cv::Point(starty, startx), cv::Scalar(255, 255, 255), &ccomp, cv::Scalar(5, 0, 0), cv::Scalar(0, 0, 0));
		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < 50; j++)
			{
				Map[i][j] = local_map.at<uchar>(i, j);
			}
		}


	if (list_target.size() == 0)
	{
		list_target.clear();
		vector<int> push(2);
		push[0] = startx;
		push[1] = starty;
		list_target.push_back(push);
		clock_t start, finish;
		start = clock();
		MTX.lock();
		DFS_search(list_target, Map);
		MTX.unlock();
		float duration = 0;
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		cout << "  duration   " << duration << endl;
		return 0;
	}

//	cout<<"  DFS-- Running len list_target "<<list_target.size()<<endl;

//	cv::imshow("DFS", local_map);
//	cv::waitKey(1);


 }

	return 0;
}



int DFS_search(vector<vector<int>> &list_target, vector<vector<int>> &Map)
{
	int i = list_target.size() - 1;

	if (Map[list_target[i][0] - 1][list_target[i][1]] == 255)
	{
		vector<int> push(2);
		push[0] = list_target[i][0] - 1;
		push[1] = list_target[i][1];
		if (std::find(list_target.begin(), list_target.end(), push) == list_target.end())
		{
			list_target.push_back(push);
			/*
			cout<<" push_back" << list_target[i][0] - 1 << "   " << list_target[i][1] << endl;
			for (int xx = 0; xx < list_target.size(); xx++)
			cout << list_target[xx][0] << " " << list_target[xx][1] << endl;
			cout << "-----------------------------------" << endl;
			*/
			DFS_search(list_target, Map);

		}

	}

	if (Map[list_target[i][0]][list_target[i][1] + 1] == 255)
	{
		vector<int> push(2);
		push[0] = list_target[i][0];
		push[1] = list_target[i][1] + 1;

		if (std::find(list_target.begin(), list_target.end(), push) == list_target.end())
		{

			list_target.push_back(push);
			DFS_search(list_target, Map);
		}

	}

	if (Map[list_target[i][0] + 1][list_target[i][1]] == 255)
	{
		vector<int> push(2);
		push[0] = list_target[i][0] + 1;
		push[1] = list_target[i][1];
		if (std::find(list_target.begin(), list_target.end(), push) == list_target.end())
		{
			list_target.push_back(push);
			DFS_search(list_target, Map);
		}

	}

	if (Map[list_target[i][0]][list_target[i][1] - 1] == 255)
	{
		vector<int> push(2);
		push[0] = list_target[i][0];
		push[1] = list_target[i][1] - 1;
		if (std::find(list_target.begin(), list_target.end(), push) == list_target.end())
		{
			list_target.push_back(push);
			DFS_search(list_target, Map);
		}

	}

	return 0;
}
