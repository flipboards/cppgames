#include "common.h"

#pragma once

#define STATE_CLOSED 0
#define STATE_OPENED 1
#define STATE_MARKED 2
#define STATE_KILLED 3

#define M_NORMAL 0
#define M_LOSE 1
#define M_WIN 2

#define DRAW_NORMAL 0
#define DRAW_ROUND 1
#define DRAW_DARK 2

#define LIST_FULLOPEN 0
#define LIST_UNOPENED 2
#define LIST_HALFOPEN 1

class lattice{

private:
	bool ismine;
	int label;
public:
	short state;
	vector<lattice*> neighbours;

	lattice();
	void setmine();
	void setneighbour(vector<lattice*> neighbour);
	void setlabel();
	int getlabel()const;
	bool open();
};
class mineMap{

	friend class solveMap;
	friend class mapDrawer;
private:
	int m_width,m_height;
	
	lattice* m_lattices;
	lattice* getLattice(int x,int y)const;
	vector<lattice*> setNeighbour(lattice* me);//The points of the lattice beside

	int m_minenumber;	//������Ŀ
	int m_opennumber;	//���׵ĸ���
	int m_lose;			//�����

	int Open(lattice* Lattice);
	int Mark(lattice* Lattice);
	int FastOpen(lattice* Lattice);
public:
	int m_mineleft;		//Left Mine Number
	mapDrawer* m_drawer;

	int Generate(int width,int height,int mineNumber,int seedX,int seedY);
	int Open(int x,int y);
	int Mark(int x,int y);
	int FastOpen(int x,int y);

	int GameState()const;
	void Draw()const;
	mineMap();
	mineMap(int width,int height);
	~mineMap();
};
class solveMap{

	mineMap* m_map;
	int mapsize;

	int* mineleft;		//ͳ�ƻ������
	int* nbleft;		//ͳ�ƻ��м�����û��, nbleft >= mineleft�������ж�����ĺ�������
	int* nbopen;		//ͳ�ƿ��˼����ף�nb - nbopen >= nbleft
	float* P;			//���ʾ�����������

	void scan();			//counting lattice information
	int justOpen();		//opening easy cases
	int getnbdetail(int index,vector<lattice*>& closednb,vector<lattice*>& openednb);
	int microProb();	//find adjacent openable lattices
	int pieceDetect(lattice* me, vector<lattice*>& closedpc, vector<lattice*>& openedpc, matrix& matcov, vector<bool>& record);
	int pieceProb();	//find continous pieces and opened them
	int finalDetect();	//detecting final situation
	int ProbExecute();	//Executing according to probability	

public:
	int Bind(mineMap*);
	int Solve(int option);
	~solveMap();
};
class mapDrawer{

	int width,height,drawflag,drawLabel;
	string window;
	mineMap* map;
private:
	Scalar getColor(int label,int colorflag);
	void paint(Mat& img,Point point,int lengthx,int lengthy,const Scalar &color,int flag);
public:
	void Bind(mineMap* map,const string window,int width,int height,int drawflag,int label);
	void Draw();
};