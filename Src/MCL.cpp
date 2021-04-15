
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

int MCL_Main(float &startx,float &starty,vector<vector<float>> &nowset, float &positionX, float &positionY)
{

	float move_x;
	float move_y;
	vector<float> max_point;
	vector<vector<float>> frame(2);
	vector<vector<float>> Map(1000, vector<float>(1000));
	vector<float> Weight;
	int step = 0;
	const int n = 500;
	ifstream myfile("Map.txt");
	Robot p[n];

	for (int i = 0; i < n; i++)
		p[i].general_location(400, 250, 750, 750);
	std::cout << "Initalize All particle ---\n";

	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			myfile >> Map[i][j];
			if (Map[i][j] == 128)
				Map[i][j] = 0;
		}
	}//¶Ámap


	Mat pic(1000, 1000, CV_8UC3, cv::Scalar(0));
	cv::namedWindow("Location", CV_WINDOW_NORMAL);
	cv::resizeWindow("Location", 600, 600);
	cv::moveWindow("Location", 0, 0);
	std::cout << "all_done" << endl;


	

	while (1)
	{

		MTX.lock();
		frame = nowset;
		MTX.unlock();
		cout << "frame size" << frame.size() << "   " << frame[0].size() << endl;


		for (int i = 0; i < 1000; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				pic.at<Vec3b>(i, j) = Map[i][j];
			}
		}

		//move_x = frame[0][0]- frameslist[step][0][0];
		//move_y = frame[1][0]- frameslist[step][1][0];
		move_x = 0;
		move_y = 0;

		//GetDistOf2DPointsSet(frame, lastframe, labels[]);
		cout << "move_x  " << move_x << " move_y " << move_y << endl;
		

		for (int i = 0; i < n; i++)
		{
			//p[i].move(move_x,move_y,1000);
			p[i].measure(frame, Map);
			pic.at<Vec3b>(p[i].x, p[i].y)[2] = 255;
			Weight.push_back(p[i].match_rate);

		}

		max_point = max(Weight, n);
		cout << max_point[0] << "  Max Weight (match_rate)" << endl;
		Weight.clear();

		for (int i = 0; i < frame[0].size(); i++)
		{
			if ((frame[0][i] + p[int(max_point[1])].x) < 1000 && (frame[0][i]+ p[int(max_point[1])].x)>0 && (frame[1][i]  + p[int(max_point[1])].y) < 1000 && (frame[1][i] + p[int(max_point[1])].y)>0)
			{
				pic.at<Vec3b>(frame[0][i] + p[int(max_point[1])].x, frame[1][i]+ p[int(max_point[1])].y)[1] = 255;
			}

		}
		MTX.lock();
		positionX = p[int(max_point[1])].x;
		positionY = p[int(max_point[1])].y;
		MTX.unlock();
		Point max_point_pos(p[int(max_point[1])].y, p[int(max_point[1])].x);
		circle(pic, max_point_pos, 3, Scalar(0, 0, 255), -1);
		



		cv::imshow("Location", pic);
		cv::waitKey(1);
		float startx, starty, limitx, limity;
		for (int i = 0; i < n; i++)
		{
			if (p[i].field > 10)
			{
				startx = p[int(max_point[1])].x - (p[i].field*0.7 / 2);
				starty = p[int(max_point[1])].y - (p[i].field*0.7 / 2);
				limitx = p[int(max_point[1])].x + (p[i].field*0.7 / 2);
				limity = p[int(max_point[1])].y + (p[i].field*0.7 / 2);
				if (startx < 0) startx = 0;
				if (starty < 0) starty = 0;
				if (limitx > 1000) limitx = 1000;
				if (limity > 1000) limity = 1000;
				p[i].general_location(startx, starty, limitx, limity);
			}
			else
			{
				startx = p[int(max_point[1])].x - 5;
				starty = p[int(max_point[1])].y - 5;
				limitx = p[int(max_point[1])].x + 5;
				limity = p[int(max_point[1])].y + 5;
				if (startx < 0) startx = 0;
				if (starty < 0) starty = 0;
				if (limitx > 1000) limitx = 1000;
				if (limity > 1000) limity = 1000;
				p[i].general_location(startx, starty, limitx, limity);

			}
		}


	}

	std::system("pause");
	return 0;
}

