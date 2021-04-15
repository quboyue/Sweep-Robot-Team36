
#include "DWA.h"
using namespace std;
using namespace cv;
#define PI acos(-1)



int DWA_MAIN_loop(vector<float> &thetas, vector<float> &dists, float &positionX, float &positionY, vector<vector<float>> &target)
{


	cv::namedWindow("DWA", CV_WINDOW_NORMAL);
	cv::resizeWindow("DWA", 400, 400);
	cv::moveWindow("DWA", 0, 600);

	vector<float> local_dists;
	vector<float> local_thetas;
	while (1)
	{
		Mat pic(1000, 1000, CV_8UC3, cv::Scalar(0));
		while (1) 
		{
			MTX.lock();
			local_dists = dists;
			local_thetas = thetas;
			if (local_thetas.size()) 
			{
				MTX.unlock();
				break;
			}

			MTX.unlock();	
		}



		DWA(target, local_dists, local_thetas, positionX, positionY, pic);
		local_dists.clear();
		local_thetas.clear();

	}

}


int DWA(vector<vector<float>> &target, vector<float> &dists, vector<float> &thetas, float &positionX, float &positionY, Mat &pic)
{


	float divied=36;
	vector<vector<float>> initalPos(2);
	vector<vector<float>> ObsDistGrade(divied);
	vector<vector<float>> RotateDistGrade(divied);
	vector<vector<float>> TotalGrades(divied);
	int roundTheta = 0;
	for (int i = 0; i < thetas.size(); i++)
	{
		roundTheta = round(thetas[i] / 360 / divied);
		if (roundTheta == divied)
			roundTheta = 0;
		ObsDistGrade[roundTheta].push_back(dists[i]);

	}

	for (float i = 0; i < ObsDistGrade.size(); i++)
	{
		float min = 350;

		for (int j = 0; j < ObsDistGrade[i].size(); j++)
		{
			if (ObsDistGrade[i][j] < min)
				min = ObsDistGrade[i][j];
		}
		ObsDistGrade[i].clear();

		line(pic, Point(positionY, positionX), Point(positionY + cos(i / 18 * PI) * min, positionX + sin(i / 18 * PI) * min), Scalar(0,100,0));

		ObsDistGrade[i].push_back(1 - (min / 350));
	}

	float angleToTarget = atan((target[0][0] - positionX) / (target[1][0] - positionY)) / PI * 180;

	if ((target[0][0] - positionX) > 0 && (target[1][0] - positionY) < 0)
		angleToTarget = angleToTarget + 180;
	if ((target[0][0] - positionX) < 0 && (target[1][0] - positionY) < 0)
		angleToTarget = angleToTarget + 180;
	if ((target[0][0] - positionX) < 0 && (target[1][0] - positionY) > 0)
		angleToTarget = -angleToTarget + 270;


	//cout << (target[0][0] - positionX) << "   (target[0][0] - positionX)" << endl;
	//cout << (target[1][0] - positionY) << "   (target[1][0] - positionY)" << endl;
	//cout << " target Angle" << angleToTarget << endl;
	angleToTarget = round(angleToTarget / 10);


	float anglecha = 0;
	for (int i = 0; i < RotateDistGrade.size(); i++)
	{

		anglecha = abs(i * 10 - (angleToTarget * 10));
		if (anglecha > 180)
			anglecha = 360 - anglecha;
		RotateDistGrade[i].push_back(anglecha / 180);
		//cout << "i " << i << " 角度得分 " << RotateDistGrade[i][0];
		//cout << " 避障度得分 " << ObsDistGrade[i][0];
		TotalGrades[i].push_back(RotateDistGrade[i][0] *  1.4+ ObsDistGrade[i][0] * 1);
		//cout << " 总得分 " << TotalGrades[i][0] << endl;
	}
	float minGrade = 2;
	float minGradeid = 0;
	for (int i = 0; i < TotalGrades.size(); i++)
	{
		if (TotalGrades[i][0] < minGrade) {
			minGrade = TotalGrades[i][0];
			minGradeid = i;
		}
	}
	//cout << " min总得分 " << minGradeid << "  " << minGrade << endl;
	line(pic, Point(positionY, positionX), Point(positionY + cos(minGradeid / 18 * PI) * 30, positionX + sin(minGradeid / 18 * PI) * 30), Scalar(0, 0, 255));
	circle(pic, Point(target[1][0], target[0][0]), 3, Scalar(0, 255, 0), -1);
	circle(pic, Point(positionY, positionX), 3, Scalar(0, 255, 0), -1);

	for (int i = 0; i < TotalGrades.size(); i++)
	{
		ObsDistGrade[i].clear();
		RotateDistGrade[i].clear();
		TotalGrades[i].clear();

	}
	imshow("DWA", pic);
	waitKey(1);


	return 0;
}
