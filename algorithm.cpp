#include"common.h"

int* randomChoose(int dataLength,int resultLength){
	int* result=new int[resultLength];
	int* list=new int[dataLength];

	for(int i=0;i<dataLength;i++)
		list[i]=i;
	int currentLength = dataLength;
	while(currentLength > dataLength-resultLength){
		int r = rand()%currentLength;
		//Swap
		int tmp = list[r];
		list[r] = list[currentLength-1];
		list[currentLength-1] = tmp;

		result[dataLength - currentLength] = list[currentLength-1];
		currentLength--;
	}
	delete list;
	return result;
}
int dectobinlist(const unsigned int num,const int length,vector<int>& result){

	int counter = 0;
	while(counter < length){
		result.push_back((num>>counter)&0x01);
		counter++;
	}
	return counter;
}
int pow(const int a,const int x){
	if(x == 0)return 1;
	else return pow(a,x-1)*a;
}
void operator+=(vector<int>& a,const vector<int>& b){

	if(a.size() != b.size())return;
	for(unsigned int i = 0;i < a.size(); i++){
		a[i] += b[i];
	}

}
int operator*(const vector<int>& a,const vector<int>& b){
	int result = 0;
	for(unsigned int i = 0; i < a.size(); i++){
		result+= a[i]*b[i];
	}
	return result;
}

int intProgram(matrix A,vector<int> B,vector<int>& X,vector<symbol> symbols){
	
	int cnt_result = 0;
	for(unsigned int cnt = 0; cnt < (unsigned)pow(2, X.size()); cnt++){		//传说中的np complete
		vector<int> x;
		dectobinlist(cnt,X.size(),x);				//将所有的情况生成对应的0-1数组
		vector<int> r = A*x;

		bool ok = true;
		for(unsigned int i = 0; i < B.size(); i++){			//检验是否满足不等式，需要满足所有才能通过
			if(symbols[i] == symbol::le)ok&=(r[i] <= B[i]);
			else if(symbols[i] == symbol::equal)ok&=(r[i] == B[i]);
		}
		if(ok){												//统计
			X += x;
			cnt_result ++;
		}
	}
	return cnt_result;	
}