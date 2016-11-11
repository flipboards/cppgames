#include "mine.h"

const char* window = "MineSweeper";
mineMap* Map;
solveMap solver;
mapDrawer drawer;
int level;
int mapWidth,mapHeight,mineNumber;
int wWidth,wHeight;
int clicktimes = 0;
int drawflag = 0;
clock_t starttime;

void Renew(){

	Map->Draw();
	if(Map->m_mineleft > 0)
		cout<<"Still "<<Map->m_mineleft<<" mines. Time used: "<<int((clock() - starttime)/1000)<<"s\r";
}
void Win(){
	
	time_t timer;	tm date;
	time(&timer);	localtime_s(&date,&timer);
	destroyWindow(window);

	if(Map->GameState() == M_WIN){
		cout<<"\nYou win! (^w^)\n";
		ofstream file("record.txt",ios::app);
		file<<date.tm_year+1900<<'/'<<(date.tm_mon+1)<<'/'<<date.tm_mday<<'\t'<<level<<'\t'<<int((clock()-starttime)/1000)<<endl;
		file.close();
		
	}
	else if(Map->GameState() ==M_LOSE){
		cout<<"\nSorry, you lose.('_') Again?\n";
	}
}
void click(int mouseEvent,int x,int y,int flags,void* param){
	
	int mapX = x*mapWidth/wWidth, mapY = y*mapHeight/wHeight;
	static long int t;
	if(t%50 == 0)Renew();
	t++;

	switch(mouseEvent){
	case CV_EVENT_LBUTTONUP:
		if(clicktimes ==0){
			Map->Generate(mapWidth,mapHeight,mineNumber,mapX,mapY);
			solver.Bind(Map);
			Map->Open(mapX,mapY);
			clicktimes = 1;
		}
		else{
			if(Map->GameState() != M_NORMAL){Win();break;}
			else{
				Map->Open(mapX,mapY);
			}
		}
		Renew();
		break;
	case CV_EVENT_RBUTTONUP:
		if(clicktimes>0)Map->Mark(mapX,mapY);
		Renew();
		break;
	case CV_EVENT_LBUTTONDBLCLK:
		if(clicktimes>0)Map->FastOpen(mapX,mapY);
		Renew();
		break;
	case CV_EVENT_MBUTTONUP:
		if(clicktimes>0)solver.Solve(1);
		Renew();
		break;
	default:
		break;
	}
}
int main(){
	srand((unsigned)time(0));
	

	while(1){
levelchoose:
		if(clicktimes !=0){
			char choice;
			cout<<"Use the last options?y/n";cin>>choice;
			if(choice == 'y')goto chooseover;
			else if(choice != 'n')goto levelchoose;
		}

		cout<<"Level: 1-basic(10x10)；2-middle(30x30)；3-difficult(100x30):";cin>>level;
		cout<<"Paintstyle: 1~6 :";cin>>drawflag;

		if(level==1){mapWidth = 10; mapHeight = 10; mineNumber = 10;}
		else if (level == 2){mapWidth = 16; mapHeight = 16; mineNumber = 40;}
		else if (level == 3){mapWidth = 30; mapHeight = 16; mineNumber = 99;}
		else {goto levelchoose;}
chooseover:		
		Map = new mineMap(mapWidth,mapHeight);
		wWidth = mapWidth*30;	wHeight = mapHeight*30;
		
		drawer.Bind(Map,window,wWidth,wHeight,(drawflag-1)/2,drawflag%2);

		Map->m_drawer = &drawer;
		clicktimes = 0;		
		starttime = 0;

		starttime = clock();
		namedWindow(window,WINDOW_NORMAL);
		resizeWindow(window,wWidth,wHeight);

		Map->Draw();
		setMouseCallback(window,click);

		waitKey(0);
		
	}
	destroyWindow(window);
	delete Map;
	
	return 1;
}
