
#include "Display.h"
using namespace cv;
std::mutex MTX;
void DDA_line(float &Startx, float &Starty, float &RealAngle, vector<float> &thetas, vector<float> &dists,Mat &pic)
{

	cv::namedWindow("Mapping", CV_WINDOW_NORMAL);
	cv::resizeWindow("Mapping", 600, 600);
	cv::moveWindow("Mapping", 600, 0);


	
		// cv::Mat img = cv::Mat::zeros(w, h, CV_8UC1);
	Mat r(pic.rows, pic.cols, CV_32FC1);
	Mat g(pic.rows, pic.cols, CV_32FC1);
	Mat b(pic.rows, pic.cols, CV_32FC1);
	Mat out[] = { b,g,r };
	float  Endx, Endy;
	float length, angle;
	float x, y, xincre, yincre;
	int ix, iy;
	float point = 0;
	//std::cout << "DDA_thread--------------------------" << thetas.size() << endl;
	int frames = 0;
	//while (1)
	if(1)
	{
		///cout << heading << "  heading" << endl;
		MTX.lock();
		vector<float> Lthetas = thetas;
		vector<float> Ldists = dists;
		MTX.unlock();

		//ofstream outfile(to_string(frames) + ".txt");
		//outfile << Startx << "  " << Starty << endl;
		for (int i = 0; i < Lthetas.size(); i++)
		{

			//std::cout << "DDA_thread" <<i<< endl;
			angle = Lthetas[i];//- RealAngle;
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
			Point p(round(y), round(x));//!研究下xy的坐标怎么回事
										//这里我对调了xy！！！
			circle(pic, p, 3, Scalar(pic.at<Vec3b>(round(y), round(x))[0], pic.at<Vec3b>(round(y), round(x))[1], 255), -1);
			
			for (int i = 0; i < k; i++)
			{
				x = x + xincre;
				y = y + yincre;
				ix = round(x);
				iy = round(y);
				if (ix != Endx && iy != Endy)
				{
					point = pic.at<Vec3b>(ix, iy)[0] / 255;
					point += log(0.3 / (1 - 0.3));
					pic.at<Vec3b>(ix, iy)[0] = point * 255;
				}
			}
			
			point = pic.at<Vec3b>(Endx, Endy)[0] / 255;
			point += log(0.7 / (1 - 0.7));
			pic.at<Vec3b>(Endx, Endy)[0] = point * 255;
			pic.at<Vec3b>(Endx, Endy)[1] =  255;
			//outfile << Endx << " " << Endy << endl;

			//
			
		}
		//outfile.close();
		//frames += 1;
		//split(pic, out);
		//b = out[0];


		imshow("Mapping",pic);
		if (waitKey(5) == 27)
		{
			imwrite("test.png", pic);
			std::cout << "保存图片" << endl;
			return;

		}
		MTX.lock();
		for (int i = 0; i < 1000; i++) 
		{
			for (int j = 0; j < 1000; j++) {
				pic.at<Vec3b>(i,j)[1] = 0;
			}
		}
		MTX.unlock();



	}

}