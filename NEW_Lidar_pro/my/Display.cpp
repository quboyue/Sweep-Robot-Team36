#include "Display.h"

void DDA_line(float &Startx, float &Starty, float &heading, vector<float> &thetas, vector<float> &dists)
{


	//cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
	int w = 1000, h = 1000;
	Mat pic(h, w, CV_8UC3, cv::Scalar(round(255 * 0.5), 0, 0)); // cv::Mat img = cv::Mat::zeros(w, h, CV_8UC1);
	float  Endx, Endy;
	float length, angle;
	float x, y, xincre, yincre;
	int ix, iy;
	float point = 0;
	//std::cout << "DDA_thread--------------------------" << thetas.size() << endl;
	int frames = 0;
	while (1)
	{
		///cout << heading << "  heading" << endl;
		MTX.lock();
		vector<float> Lthetas = thetas;
		vector<float> Ldists = dists;
		MTX.unlock();
		ofstream outfile(to_string(frames) + ".txt");
		outfile << Startx << "  " << Starty << endl;
		for (int i = 0; i < Lthetas.size(); i++)
		{

			//std::cout << "DDA_thread" <<i<< endl;
			angle = Lthetas[i] + heading;
			length = Ldists[i];
			Endx = round(length * sin(angle* PI / 180) + Startx);
			Endy = round(length * cos(angle * PI / 180) + Starty);
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
			outfile << Endx << " " << Endy << endl;

			//

		}
		outfile.close();
		frames += 1;
		imshow("test", pic);
		if (waitKey(500) == 27)
		{
			imwrite("test.png", pic);

			std::cout << "保存图片" << endl;
			return;

		}





	}

}
