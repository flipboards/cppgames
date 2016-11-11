#include<iostream>
#include<sstream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<vector>
#include<fstream>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>

#define cvWhite Scalar(255,255,255)
#define cvBlack Scalar(0,0,0)
#define cvGreen Scalar(0,255,0)
#define cvYellow Scalar(0,255,255)
#define cvRed Scalar(0,0,255)

#define Min(a,b) (a)<(b)?(a):(b)
#define Max(a,b) (a)>(b)?(a):(b)

using namespace std;
using namespace cv;

enum symbol{
	equal,greater,less,ge,le
};

//Generate Random Sequence without Repeat
int* randomChoose(int dataLength,int resultLength);
//Convert Other Type into String using StringStream
template<typename type>
string toString(type x){
	stringstream s;
	s<<x;
	string str;
	s>>str;
	return str;
}

//Convert a decimal number to a binary list
int dectobinlist(const unsigned int num,const int length, vector<int>& result);

//vector calculation
void operator+=(vector<int>& a,const vector<int>& b);
int operator*(const vector<int>& a,const vector<int>& b);

template<typename type>
int pushnew(vector<type>& v, const type data) {
	if (find(v.begin(), v.end(), data) == v.end()) {
		v.push_back(data); return 1;
	}
	else return 0;
}

struct matrix{
	vector<vector<int>> data;
	matrix(int row,int col,int datainit = 0){
		data = vector<vector<int>>(row,vector<int>(col,datainit));
	}
	vector<int> operator*(vector<int> v){
		vector<int> result;
		for(unsigned int i = 0; i < data.size(); i++){
			result.push_back(data[i]*v);
		}
		return result;
	}
	void addline(vector<int>& line) {
		data.push_back(line);
	}
};

//Solve Inequations by only 0 and 1. X is the total frequency
int intProgram(matrix A,vector<int> B,vector<int>& X,vector<symbol> symbols);

int floatProgram(matrix A, vector<int> B, vector<float>& X);
