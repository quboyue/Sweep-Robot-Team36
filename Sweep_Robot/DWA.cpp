
#include "DWA.h"
using namespace std;
using namespace cv;
#define PI acos(-1)

float angle2target(vector<vector<int>> &target,float &positionX,float &positionY)
{

	float angleToTarget = atan((target[0][0] - positionX) / (target[1][0] - positionY)) / PI * 180;
	if ((target[1][0] - positionY) < 0)
		angleToTarget = angleToTarget + 180;

	return angleToTarget;
}




int DWA_MAIN_loop(vector<vector<float>>  &nowset, float &positionX, float &positionY, vector<vector<int>> &list_target,float &RealAngle, float &targetAngle)
{
	//cv::namedWindow("DWA", CV_WINDOW_NORMAL);
	//cv::resizeWindow("DWA", 450, 450);
	//cv::moveWindow("DWA", 450, 450);

	vector<vector<int>> target(2,vector<int>(1));
	float dist2target;
	vector<vector<float>> local_nowset;
	while (1)
	{
		Mat pic(1000, 1000, CV_8UC3, cv::Scalar(0));
		while (1)
		{
			MTX.lock();
			if (list_target.size()>1) 
			{
				target[0][0] = list_target[0][0]*20;
				target[1][0] = list_target[0][1]*20;
				dist2target = sqrt(pow(target[0][0] - positionX, 2) + pow(target[1][0] - positionY, 2));
				while (dist2target < 15)
				{
					vector<vector<int>>::iterator first = list_target.begin();
					list_target.erase(first);
					target[0][0] = list_target[0][0]*20;
					target[1][0] = list_target[0][1]*20;
					dist2target = sqrt(pow(target[0][0] - positionX, 2) + pow(target[1][0] - positionY, 2));
				}
			}
			local_nowset = nowset;

			if (local_nowset[0].size()>0 &&list_target.size()>1)
			{
				MTX.unlock();
				break;
			}
			MTX.unlock();
			waitKey(200);
		}
		//target[1][0] = 600;
		//target[0][0] = 600;
		//std::cout << " here 1" << endl;
		DWA(target, local_nowset, positionX, positionY, pic, RealAngle,targetAngle,dist2target);
		//std::cout << " here 2" << endl;

	}
}

int DWA(vector<vector<int>> &target, vector<vector<float>> &nowset, float &positionX, float &positionY, Mat &pic,float &RealAngle,float &targetAngle,float &dist2target)
{
	float heading = RealAngle;
	heading -= 90;
	if (heading < 0) heading += 360;
	heading = round(heading);

	int divied=36;
	vector<float> ObsDistGrade(divied*2);
	vector<float> RotateDistGrade;
	vector<float> TotalGrades;
	vector<float> Angles;
	Mat show(1000, 1000, CV_8UC3, Scalar(0, 0, 0));

	for (int i =-divied; i < divied; i++)
	{

		float lineAngle = heading + i;
		if (lineAngle < 0) lineAngle += 360;
		if (lineAngle > 360) lineAngle -= 360;
		Angles.push_back(lineAngle);

		float anangle2targetResult = angle2target(target, positionX, positionY);
		float AngleGrade = abs(lineAngle - anangle2targetResult);
		while (AngleGrade > 180) AngleGrade = 360 - AngleGrade;
		AngleGrade=abs(AngleGrade);
		RotateDistGrade.push_back(1-(AngleGrade/150));

		ObsDistGrade[i + divied] = -10;
		float min_Obs = 350;
		MTX.lock();

		float test_x = sin(lineAngle / 180 * PI) * 10;
		float test_y = cos(lineAngle / 180 * PI) * 10;
			for (int j = 0; j < nowset[0].size(); j++) 
		{
			float distance = pow((nowset[0][j] - test_x),2)+ pow((nowset[1][j] - test_y), 2);
			distance = sqrt(distance);
			if (distance <min_Obs ) 
			{
				//std::cout << " heading "<< heading <<"  "<<lineAngle << "  "<<i<<" distance " << distance << endl;
				min_Obs = distance;
			}
		}
		MTX.unlock();
		ObsDistGrade[i + divied] = (min_Obs);

	}


	for (int i = 0; i < ObsDistGrade.size(); i++)
	{
		//cout << ObsDistGrade[i] <<"  max-min " <<(max_ObsDistGrade - min_ObsDistGrade) << endl;
		// ObsDistGrade[i]= ObsDistGrade[i]/(max_ObsDistGrade)*1.7;
		ObsDistGrade[i] = log(ObsDistGrade[i]/10);
	}

	float max = 0;
	int id=0;
	for (int i = 0; i < divied*2; i++)
	{
		if ((ObsDistGrade[i] + RotateDistGrade[i]) > max)
		{
			max = ObsDistGrade[i] + RotateDistGrade[i];
			id = i;
		}
//		cout << heading << "  RotateDistGrade " << (1- RotateDistGrade[i])*150.0 << "  ObsDistGrade  " << ObsDistGrade[i] << " dist2target " << dist2target << "  " << id << endl;
		cout << i << "  RotateDistGrade " << RotateDistGrade[i]  << "  ObsDistGrade  " << ObsDistGrade[i] <<" dist2target "<< dist2target <<"  "<< id <<endl;
	}

	if (max < 0) id = divied * 2 - 1; //emergency turn to right when there are no way in front
	targetAngle = Angles[id]+90;
	if (targetAngle > 360) targetAngle -= 360;
	waitKey(100);
	cout << " DWA--  Target Angle  "<< targetAngle << endl;
	return 0;
}

