#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<fstream>
#include<iostream>
using namespace std;

int s[4][4];
int i,j,x,y,realchange;
int wintimes,alltimes;
void change(int drc, int a,int b){
	int t;

	     if(drc==1){t=s[a][b];s[a][b]=s[a-1][b];s[a-1][b]=t;}
	else if(drc==2){t=s[a][b];s[a][b]=s[a+1][b];s[a+1][b]=t;}
	else if(drc==3){t=s[a][b];s[a][b]=s[a][b-1];s[a][b-1]=t;}
	else if(drc==4){t=s[a][b];s[a][b]=s[a][b+1];s[a][b+1]=t;}
	
}
void pop(int drc,int a, int b)
{int t;
 t=s[a][b];
 realchange=0;
	     if(drc==1&&s[a-1][b]==t){s[a-1][b]=2*t;s[a][b]=0;realchange=1;}
	else if(drc==2&&s[a+1][b]==t){s[a+1][b]=2*t;s[a][b]=0;realchange=1;}
	else if(drc==3&&s[a][b-1]==t){s[a][b-1]=2*t;s[a][b]=0;realchange=1;}
	else if(drc==4&&s[a][b+1]==t){s[a][b+1]=2*t;s[a][b]=0;realchange=1;}

}
void popall(int drc){
	for (j=0;j<=3;j++)
		for (i=1;i<=3;i++)
        {if(drc==75){pop(1,i,j);}
	else if(drc==77){pop(2,3-i,j);}
	else if(drc==72){pop(3,j,i);}
	else if(drc==80){pop(4,j,3-i);}
	}
}
void set(){
	
	for (j=0;j<=3;j++)
		{for(i=0;i<=3;i++)
		  {cout<<s[i][j]<<"    ";}
	    cout<<endl<<endl;}
		
    cout<<endl;}

void gravity(int num){
int n;
for(n=0;n<=3;n++)
	for(i=3;i>=1;i--)
		for(j=1;j<=i;j++)
         {       if (num==75&&s[j-1][n]==0)change(1,j,n);
		    else if (num==72&&s[n][j-1]==0)change(3,n,j);
		    else if (num==77&&s[4-j][n]==0)change(2,3-j,n);
		    else if (num==80&&s[n][4-j]==0)change(4,n,3-j);
			}
}

void keypress(int num){
	gravity(num);popall(num);gravity(num);
}

int full(){
	int k;
	k=2;
	for(i=0;i<=3;i++)
		for(j=0;j<=3;j++)
	        {if (s[i][j]==0){k=0;break;}
	        else if (s[i][j]==2048){k=1;break;}}
	return k;
}

void randomize(){

 srand((int)time(0));
 for (i=0;i<=3;i++)
	for(j=0;j<=3;j++)
		 {s[i][j]=0;}
}

void ran(){
	if(realchange=1){
	x=rand()%4;y=rand()%4;
	while(s[x][y]!=0){x=rand()%4;y=rand()%4;}
	if(rand()>0.2)s[x][y]=2;	
	else s[x][y]=4;
	}
}
int reading(int rec,bool win){
	int record;
	ifstream infile;
	ofstream outfile;
	record=0;
	if (rec==0){
	infile.open("c:\\2048log.log");
	if(!infile.is_open()){
		outfile.open("C:\\2048log.log");
		outfile<<0<<endl<<0<<endl<<0;
		outfile.close();
		infile.open("c:\\2048log.log");}
	infile>>record>>wintimes>>alltimes;
	infile.close();
	return record;
	}
	else{
		if(win)wintimes=wintimes+1;
		else rec=0;
		alltimes=alltimes+1;
		outfile.open("C:\\2048log.log");
		outfile<<rec<<endl<<wintimes<<endl<<alltimes;
		outfile.close(); 
	return 0;}
}