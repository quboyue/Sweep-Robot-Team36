
#include "MCL.h"
#include "Display.h"
using namespace cv;
using namespace std;
#define PI acos(-1)
std::random_device rd; 
std::default_random_engine gen{ rd() };

float  mod(float first_term, float second_term)
{
	return (int(first_term * 1000) % int(second_term * 1000)) / 1000.0f;
}
double gen_real_random()
{
	// Generate real random between 0 and 1
	uniform_real_distribution<double> real_dist(0.0, 1.0); //Real
	return real_dist(gen);
}

class Robot {
public:
	float x, y, orient; //robot poses
	float match_rate;
	float field;
	Robot()
	{
		x = 0;
		y = 0;
		orient = 0;
		match_rate = 0;
	}
	void general_location(float startx, float starty, float limitx, float limity)
	{
		x = gen_real_random() *(limitx - startx) + startx;
		y = gen_real_random() *(limity - starty) + starty;
		//orient = gen_real_random() * 2.0 * PI; // robot's orientation
		field = limitx - startx;
	}

	void move(float add_x, float add_y, int world_size)
	{
		x = mod(x + add_x, world_size);
		y = mod(y + add_y, world_size);
	}


	void measure(vector<vector<float>> &frame, vector<vector<float>> &Map)
	{
		float all_match = 0;
		float Map_point;
		int world_size = Map.size();
		int dishu = 0;
		for (int i = 0; i < frame[0].size(); i++)
		{
			if ((frame[0][i]+ x) < world_size && (frame[0][i]+ x)>0 && (frame[1][i]+ y) < world_size && (frame[1][i] + y)>0)
			{
				Map_point = Map[frame[0][i]+ x][frame[1][i]+ y];
				all_match += Map_point;
			}
			else
			{
				all_match -= 100;
			}

		}

		match_rate = all_match;
	}
};


vector<float> max(vector<float> Weight, int n)
{
	float max = 0;
	int id = 0;
	for (int i = 0; i < n; i++) 
	{
		if (Weight[i] > max)
		{
			max = Weight[i];
			id = i;
		}
	}
	vector<float> result(2);
	result[0] = max;
	result[1] = id;
	//cout << "  maxid  " << id << endl;
	return result;
}

int MCL_Main(vector<vector<float>> &nowset, float &positionX, float &positionY,bool &MCL_ready)
{


	vector<float> max_point;
	vector<vector<float>> frame(2);
	vector<vector<float>> Map(1000, vector<float>(1000));
	vector<float> Weight;
	int step = 0;
	const int n = 250;

	Robot p[n];

	for (int i = 0; i < n; i++)
		p[i].general_location(400, 250, 750, 750);

	Mat B_channel = cv::imread("test2.png");

	vector<Mat> channels;
	split(B_channel, channels);

	B_channel = channels.at(0);
	cv::blur(B_channel, B_channel, cv::Size(3, 3), cv::Point(-1, -1));

	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			Map[i][j] = B_channel.at<uchar>(i,j);
			if (Map[i][j] == 128)
				Map[i][j] = 0;
		}
	}
	



	cv::namedWindow("Location", CV_WINDOW_NORMAL);
	cv::resizeWindow("Location", 450, 450);
	cv::moveWindow("Location", 0, 0);
	//std::cout << "all_done" << endl;



	

	while (1)
	{
	

		while (1)
		{
			MTX.lock();
			frame = nowset;
			MTX.unlock();
			if (frame[0].size()) 
				break;
		}

		Mat G_channel(1000, 1000, CV_8UC1, cv::Scalar(0));
		Mat R_channel(1000, 1000, CV_8UC1, cv::Scalar(0));


		for (int i = 0; i < n; i++)
		{
			//p[i].move(move_x,move_y,1000);
			p[i].measure(frame, Map);
			R_channel.at<uchar>(p[i].x, p[i].y) = 255;
			Weight.push_back(p[i].match_rate);
		}

		max_point = max(Weight, n);
		Weight.clear();

		MTX.lock();
		positionX = p[int(max_point[1])].x;
		positionY = p[int(max_point[1])].y;
		MTX.unlock();

		for (int i = 0; i < frame[0].size(); i++)
		{
			if ((frame[0][i] + positionX) < 1000 && (frame[0][i]+ positionX)>0 && (frame[1][i]  + positionY) < 1000 && (frame[1][i] + positionY)>0)
			{
				G_channel.at<uchar>(frame[0][i] + positionX, frame[1][i]+ positionY)= 255;
			}

		}



	   circle(R_channel, Point(positionY, positionX), 3,255, -1);
	   cv::putText(G_channel, "Position "+to_string(positionX)+"  "+to_string(positionY), cv::Point(0, 50), cv::FONT_HERSHEY_TRIPLEX, 2, cv::Scalar(255), 0.001, CV_AA);
	   vector<Mat> MergeList= { B_channel,G_channel,R_channel };
	   Mat MergePic;

		cv::merge(MergeList, MergePic);
		cv::imshow("Location", MergePic);
		cv::waitKey(1);

		float startx, starty, limitx, limity;
		for (int i = 0; i < n; i++)
		{
			if (p[i].field > 10)
			{
				startx = positionX - (p[i].field*0.9 / 2);
				starty = positionY - (p[i].field*0.9 / 2);
				limitx = positionX + (p[i].field*0.9 / 2);
				limity = positionY + (p[i].field*0.9 / 2);
				if (startx < 0) startx = 0;
				if (starty < 0) starty = 0;
				if (limitx > 1000) limitx = 1000;
				if (limity > 1000) limity = 1000;
				p[i].general_location(startx, starty, limitx, limity);
			}
			else
			{
				MCL_ready = true;
				startx = positionX - 5;
				starty = positionY - 5;
				limitx = positionX + 5;
				limity = positionY + 5;
				if (startx < 0) startx = 0;
				if (starty < 0) starty = 0;
				if (limitx > 1000) limitx = 1000;
				if (limity > 1000) limity = 1000;
				p[i].general_location(startx, starty, limitx, limity);

			}
		}

	}

	
	return 0;
}

