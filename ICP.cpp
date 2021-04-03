#include "ICP.h"
#define PI acos(-1)



/*
void thread_find_k_nearests(tree_model *model, vector<vector<float>> A, float point, float *loss)
{
float test[2];
size_t args[1];
float dists[1];
for (int i = point; i < point + 20; i++)
{
if (i == A[0].size())
break;
test[0] = A[0][i];
test[1] = A[1][i];
find_k_nearests(model, test, 1, args, dists);
*loss += dists[0];
}
}
void thread_delta_find_k_nearests(tree_model *model, vector<vector<float>> A, float point, float *loss, float data[], float *lossx, float *lossy)
{
float test[2];
size_t args[1];
float dists[1];
for (int i = point; i < point + 20; i++)
{
if (i == A[0].size())
break;
test[0] = A[0][i];
test[1] = A[1][i];
find_k_nearests(model, test, 1, args, dists);
*lossx += test[0] - data[args[0] * 2];
*lossy += test[1] - data[args[0] * 2 + 1];
}
}
void thread_BA_find_k_nearests(tree_model *model, vector<vector<float>> B, float point, float data[], float *t_nume, float *t_deno)
{

size_t args[1];
float dists[1];
float p[2];
int j;
for (int i = point; i < point + 20; i++)
{
if (i == B[0].size())
break;
p[0] = B[0][i];
p[1] = B[1][i];
//j = GetClosestID(p, A);  //找到距离最近的目标点云A点id
find_k_nearests(model, p, 1, args, dists);
j = args[0];
*t_nume += data[j * 2 + 1] * p[0] - data[j * 2] * p[1];
*t_deno += data[j * 2] * p[0] + data[j * 2 + 1] * p[1];//获得求和公式，分母的一项

}
}
*/



vector<vector<float>> Matmul(vector<vector<float>> A, vector<vector<float>> B)
{
	vector<vector<float>> result(A.size(), vector <float>(B[0].size()));

	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < B[0].size(); j++)
		{
			for (int k = 0; k < A[0].size(); k++)
			{
				result[i][j] += A[i][k] * B[k][j];
			}

		}

	}
	return result;

}

vector<vector<float>> operator-(vector<vector<float>> A, vector<vector<float>> A_mean)
{
	// 假设 v1.size() == v2.size()
	vector<vector<float>> Result(A.size(), vector<float>(A[0].size()));

	for (int i = 0; i < A[0].size(); i++)
	{

		Result[0][i] = (A[0][i] - A_mean[0][0]);
		Result[1][i] = (A[1][i] - A_mean[1][0]);
	}
	return Result;

}

vector<vector<float>> operator+(vector<vector<float>> A, vector<vector<float>> A_mean)
{
	// 假设 v1.size() == v2.size()

	vector<vector<float>> Result(A.size(), vector<float>(A[0].size()));

	for (int i = 0; i < A[0].size(); i++)
	{

		Result[0][i] = (A[0][i] + A_mean[0][0]);
		Result[1][i] = (A[1][i] + A_mean[1][0]);
	}
	return Result;

}


vector<vector<float>> Mean(vector<vector<float>> set)
{
	float totalline1 = 0.0, totalline2 = 0.0;
	vector<vector<float>> result(2, vector<float>(1));
	for (int i = 0; i < set[0].size(); i++)
	{
		totalline1 += set[0][i];
		totalline2 += set[1][i];
	}
	result[0][0] = totalline1 / set[0].size();
	result[1][0] = totalline2 / set[0].size();
	return result;
}




float GetDistOf2DPointsSet(vector<vector<float>> &A, vector<vector<float>> &B, float labels[])
{	//0.17s
	size_t args[1];
	float dists[1];
	float loss = 0;
	//cout << "我是主线程" << endl;
	//int a = 9;
	//thread th2(proc, a);
	//th2.join();

	float data[1000];
	for (int i = 0; i < B[0].size(); i++) //B[0].size()
	{
		data[i * 2] = B[0][i];
		data[i * 2 + 1] = B[1][i];
	}


	tree_model *model = build_kdtree(data, labels, B[0].size(), 2, 2);//0.08



	float test[2];

	for (int i = 0; i <A[0].size(); i++)
	{
		test[0] = A[0][i];
		test[1] = A[1][i];
		find_k_nearests(model, test, 1, args, dists);
		loss += dists[0];


	}//0.05s

	free_tree_memory(model->root);
	return loss / A[0].size();
}



vector<vector<float>>  GetDelta_T(vector<vector<float>> &A, vector<vector<float>> &B, float labels[])
{
	float loss = 0;
	float data[1000];
	vector<float> lossx ; 
	vector<float>  lossy;
	float lossx_all=0;
	float lossy_all=0;
	for (int i = 0; i < B[0].size(); i++)
	{
		data[i * 2] = B[0][i];
		data[i * 2 + 1] = B[1][i];
	}
	tree_model *model = build_kdtree(data, labels, B[0].size(), 2, 2);
	size_t args[1];
	float dists[1];
	float test[2];
	int earse_number=0;
	for (int i = 0; i <A[0].size(); i++)
	{
		test[0] = A[0][i];
		test[1] = A[1][i];
		find_k_nearests(model, test, 1, args, dists);
		lossx.push_back(test[0] - data[args[0] * 2]);
		lossy.push_back( test[1] - data[args[0] * 2 + 1]);
		//cout <<i<< "  去除之前点差距 " << test[0] - data[args[0] * 2]<< "  " << test[1] - data[args[0] * 2 + 1]<< endl;

	}
	
	for (int i = 0; i < A[0].size() / 15; i++) 
	{
		earse_number += 2;
		auto maxPositionX = max_element(lossx.begin(), lossx.end());
		auto minPositionX = min_element(lossx.begin(), lossx.end());
		//cout << *maxPositionX << " at the postion of " << maxPositionX - lossx.begin() << endl;
		//cout << *minPositionX << " at the postion of " << minPositionX - lossx.begin() << endl;

		lossx[maxPositionX - lossx.begin()]=0;
		lossy[maxPositionX - lossx.begin()] = 0;
		lossx[minPositionX - lossx.begin()] = 0;
		lossy[minPositionX - lossx.begin()] = 0;

		
	}

	for (int i = 0; i < A[0].size() / 15; i++)
	{
		earse_number += 2;
		auto maxPositionY = max_element(lossy.begin(), lossy.end());
		auto minPositionY = min_element(lossy.begin(), lossy.end());
		//cout << *maxPositionY << " at the postion of " << maxPositionY - lossy.begin() << endl;
		//cout << *minPositionY << " at the postion of " << minPositionY - lossy.begin() << endl;

		lossx[maxPositionY - lossy.begin()] = 0;
		lossy[maxPositionY - lossy.begin()] = 0;
		lossx[minPositionY - lossy.begin()] = 0;
		lossy[minPositionY - lossy.begin()] = 0;
	}

	for (int i = 0; i < lossx.size(); i++) 
	{
		lossx_all += lossx[i];
		lossy_all += lossy[i];
	
	}


	vector<vector<float>> result(2, vector<float>(1));
	result[0][0] = lossx_all / (lossx.size()-earse_number);
	result[1][0] = lossy_all / (lossy.size()-earse_number);
	cout << "    平均点差距 " << result[0][0] << "  " << result[1][0]<< endl;
	free_tree_memory(model->root);
	return result;
}



vector<vector<vector<float>>> ICP_D(vector<vector<float>> A, vector<vector<float>> B)
{


	int iteration_times = 0;
	float theta;
	
	float dist_now = 1;  // A, B两点集之间初始化距离
	float dist_improve = 1;  // A, B两点集之间初始化距离提升
	float dist_before = 0;
	
	float LastRealAngle=0;
	float p[2];
	//int j;
	float t_nume = 0, t_deno = 0;
	vector<vector<float>> delta_R(2, vector<float>(2));
	vector<vector<float>> delta_T(2, vector<float>(1));
	vector<vector<float>> R(2, vector<float>(2));
	R[0][0] = 1.0; R[0][1] = 0.0;
	R[1][0] = 0.0; R[1][1] = 1.0;
	vector<vector<float>> T(2, vector<float>(1));
	T[0][0] = 0;
	T[1][0] = 0;
	float loss = 0;
	float data[1000];
	float labels[1000];
	for (int i = 0; i < 360; i++)
	{
		labels[i] = (float)i;
	}
	



		delta_T = GetDelta_T(A,  B, labels);//0.06
		B =  B + delta_T;
		T = T + delta_T;
		iteration_times += 1;

	
	vector<vector<vector<float>>> result_all(2);
	result_all[0] = R;
	result_all[1] = T;
	return result_all;



}

/*
for (int i = 0; i < A[0].size(); i++)
{
data[i * 2] = A[0][i];
data[i * 2 + 1] = A[1][i];
}

//float dist_before = GetDistOf2DPointsSet(A, B, labels); // A, B两点集之间距离
//tree_model *model = build_kdtree(data, labels, A[0].size(), 2, 2);
//size_t args[1];
//float dists[1];
*/
/*

while (iteration_times < 3 && dist_now > 0.01)
{

t_nume = 0, t_deno = 0;


for (int i = 0; i < B[0].size(); i++)
{
p[0] = B[0][i];
p[1] = B[1][i];
//j = GetClosestID(p, A);  //找到距离最近的目标点云A点id
//find_k_nearests(model, p, 1, args, dists);
j = args[0];
t_nume += data[j * 2 + 1] * p[0] - data[j * 2] * p[1];
t_deno += data[j * 2] * p[0] + data[j * 2 + 1] * p[1];//获得求和公式，分母的一项

}//0.06



theta = theta / 180 * PI;
theta = 0;
cout << " Real Angle " << theta <<"  "<< RealAngle<< endl;
delta_R[0][0] = cos(theta); delta_R[0][1] = -sin(theta);
delta_R[1][0] = sin(theta); delta_R[1][1] = cos(theta);
LastRealAngle = RealAngle;

delta_T = GetDelta_T(A, Matmul(delta_R, B), labels);//0.06

B = Matmul(delta_R, B) + delta_T;
R = Matmul(delta_R, R);
T = Matmul(delta_R, T) + delta_T;
iteration_times += 1; //迭代次数 + 1

dist_now = GetDistOf2DPointsSet(A, B, labels); //0.06
//dist_improve = dist_before - dist_now;// 计算这一次迭代两个点云之间缩短的距离
dist_before = dist_now;  // 将"现在距离"赋值给"以前距离"
//printf("     迭代：第%d次，距离：%3.2f，缩短：%3.2f \n", iteration_times, dist_now, dist_improve);




}
*/