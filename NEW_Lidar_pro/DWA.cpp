
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
	cv::namedWindow("DWA", CV_WINDOW_NORMAL);
	cv::resizeWindow("DWA", 450, 450);
	cv::moveWindow("DWA", 450, 450);

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

		RotateDistGrade.push_back(1-(AngleGrade/150));
		line(show, Point(positionY, positionX), Point(positionY + cos(anangle2targetResult / 180.0*PI)*200, positionX + sin(anangle2targetResult / 180.0*PI) * 200), Scalar(0, 0, 250),3);
		line(show, Point(positionY, positionX), Point(positionY + cos(heading / 180.0*PI)*200, positionX + sin(heading / 180.0*PI) * 200), Scalar(255, 0, 0),4);
		ObsDistGrade[i + divied] = -10;
		float min_Obs = 350;
		MTX.lock();

		float test_x = sin(lineAngle / 180 * PI) * 10;
		float test_y = cos(lineAngle / 180 * PI) * 10;
		cv::circle(show, Point(positionY+test_y, positionX+test_x), 1, Scalar(250, 0, 0), 1);

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


	float min_ObsDistGrade = 350;
	float max_ObsDistGrade = 0;
	for (int i = 0; i < ObsDistGrade.size(); i++) 
	{
		if (ObsDistGrade[i] < min_ObsDistGrade)
			min_ObsDistGrade = ObsDistGrade[i];

		if (ObsDistGrade[i] > max_ObsDistGrade)
			max_ObsDistGrade = ObsDistGrade[i];
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
		cout << i << "  RotateDistGrade " <<  RotateDistGrade[i] << "  ObsDistGrade  " << ObsDistGrade[i] << " dist2target " << dist2target << "  " << id << endl;
		//cout << i << "  RotateDistGrade " << (1-RotateDistGrade[i])*72  << "  ObsDistGrade  " << ObsDistGrade[i]*350.0 <<" dist2target "<< dist2target <<"  "<< id <<endl;
	}
	
	//cout << " hereeee 5" << endl;
	line(show, Point(positionY, positionX), Point(positionY + cos(Angles[id] / 180.0*PI) * 200, positionX + sin(Angles[id] / 180.0*PI) * 200), Scalar(255, 255, 255), 4);
	cv::circle(show, Point(target[1][0], target[0][0]), 6, Scalar(255,0, 0), 3);
	cv::putText(show, to_string(Angles[id])+" "+to_string(ObsDistGrade[id])+" "+to_string(RotateDistGrade[id] ), cv::Point(0, 50), cv::FONT_HERSHEY_TRIPLEX, 2, cv::Scalar(255,0,0), 0.001, CV_AA);

	//RED is Goal Blue is heading  Write is plan
	if (max < 0) id = divied * 2 - 1; //emergency turn to right when there are no way in front
	targetAngle = Angles[id];
	//if (targetAngle > 360) targetAngle -= 360;
	cv::putText(show, "Target " + to_string(targetAngle) + " Heading " + to_string(RealAngle), cv::Point(50, 100), cv::FONT_HERSHEY_TRIPLEX, 1, cv::Scalar(0, 0, 250), 0.001, CV_AA);

	imshow("DWA", show);
	waitKey(150);
	cout << " DWA--  Target Angle  "<< targetAngle << endl;
	return 0;
}
