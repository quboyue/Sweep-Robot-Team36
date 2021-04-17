#include <opencv2/highgui/highgui_c.h>
#include "Display.h"
using namespace cv;
std::mutex MTX;
void DDA_line(float &Startx, float &Starty, vector<float> &thetas, vector<float> &dists,Mat &pic,bool &MCL_ready)
{
//	cv::namedWindow("Mapping", CV_WINDOW_NORMAL);
//	cv::resizeWindow("Mapping", 450, 450);
//	cv::moveWindow("Mapping", 450, 0);

	float  Endx, Endy;
	float length, angle;
	float x, y, xincre, yincre;
	float point = 0;
	int frames = 0;

	while (1)
	{
		cv::waitKey(100);
//		cout<<" DDA_Line Running "<<MCL_ready<<endl;
		if (MCL_ready) 
		{
	
			Mat G_channel(1000, 1000, CV_8UC1, cv::Scalar(0));
			Mat R_channel(1000, 1000, CV_8UC1, cv::Scalar(0));
			MTX.lock();
			vector<float> Lthetas = thetas;
			vector<float> Ldists = dists;
			MTX.unlock();

			MTX.lock();
	
			for (int i = 0; i < Lthetas.size(); i++)
			{
				angle = Lthetas[i];
				length = Ldists[i];
				Endx = round(length * sin(angle* PI / 180) + Startx);
				Endy = round(length * cos(angle* PI / 180) + Starty);
				x = Startx;
				y = Starty;
				int k = abs(Endx - x);
				if (abs(Endy - y) > k)
				{
					k = abs(Endy - y);
				}
				xincre = (float)(Endx - x) / k;
				yincre = (float)(Endy - y) / k;
		
				//这里我对调了xy！！！
				circle(R_channel, Point(round(y), round(x)), 3, Scalar(255), -1);
				for (int i = 0; i < k; i++)
				{
					x = x + xincre;
					y = y + yincre;
					//这里没做函数越界检查 一般不会越界 除非定位错了
					if (round(x) != Endx &&  round(y) != Endy)
					{
						point = pic.at<uchar>(round(x), round(y))/ 255.0;
						point += log(0.3 / (1 - 0.3));
						if (point < 0) point = 0;
						pic.at<uchar>(round(x), round(y)) = point * 255;
					}
				}
	
				point = pic.at<uchar>(Endx, Endy) / 255.0;
				point += log(0.7 / (1 - 0.7));
				if (point < 0) point = 0;
				pic.at<uchar>(Endx, Endy)= point * 255;
				G_channel.at<uchar>(Endx, Endy)= 255;
			}
			MTX.unlock();

			vector<Mat> MergeList = { pic,G_channel,R_channel };
			Mat MergePic;
			cv::merge(MergeList, MergePic);
			//cv::imshow("Mapping", MergePic);
			if (waitKey(100) == 27)
			{
				imwrite("test3.png", MergePic);
				std::cout << "Save pic" << endl;
				return;
			}

		}
	}
}
