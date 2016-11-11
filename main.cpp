#include<conio.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<fstream>
using namespace std;

//-----definitions---------
#define width 4
#define height 4
#define winnumber 2048
#define unit 2

#define left 0x01
#define right 0x02
#define up 0x03
#define down 0x04

#define result_normal 0
#define result_win 1
#define result_lose	2

#define FourAppearRate 5
//-----end definitions-----

//-----global variables----
int s[width][height]{ 0 };
int x, y; bool changed;
int wintimes, alltimes;
//-----end global variables--

void move(int& src, int& dst) {
//	changed = false;
	if (dst == 0) {
		dst = src; src = 0;
		changed = true;
	}
}
void pop(int& src, int& dst) {
//	changed = false;
	if (src == dst) {
		dst += src;
		src = 0;
//		changed = true;
	}

}
void popall(int drc) {
	for (int j = 0; j < height; j++)
		for (int i = 1; i < width; i++) {
			if (drc == left)pop(s[i][j], s[i - 1][j]);
			else if (drc == right)pop(s[width - 1 - i][j], s[width - i][j]);
			else if (drc == up)pop(s[j][i], s[j][i - 1]);
			else if (drc == down)pop(s[j][width - 1 - i], s[j][width - i]);
		}
}
//the free move of blocks
void gravity(int direction) {

	changed = false;		
	for (int n = 0; n <= 3; n++)
		for (int i = width - 1; i >= 1; i--)
			for (int j = 1; j <= i; j++)
			{
				if (direction == left)move(s[j][n], s[j - 1][n]);
				else if (direction == up)move(s[n][j], s[n][j - 1]);
				else if (direction == right)move(s[width - 1 - j][n], s[width - j][n]);
				else if (direction == down)move(s[n][width - 1 - j], s[n][width - j]);
			}
}
//judge the game state
int judge() {

	int result = result_lose;
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			if (s[i][j] == 0) { 
				result = result_normal; break; }
			else if (s[i][j] == winnumber) return result_win;
		}
	return result;
}
//generate new blocks
void generate() {
	if (changed) {
		x = rand() % width; y = rand() % height;
		while (s[x][y] != 0) {
			x = rand() % width; y = rand() % height;
		}
		if (rand()% FourAppearRate > 1)s[x][y] = unit;
		else s[x][y] = unit * 2;
	}
}
//print out
void print() {

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			cout << s[i][j] << "\t";
		}
		cout << "\n\n";
	}
	cout << endl;
}
//open record file and save
void fileIO(int& record, bool win) {
	fstream file;

	//if no record, open and read
	if (!record) {
		file.open("2048log.log", ios_base::in);
		if (!file.is_open()) {
			file.open("2048log.log", ios_base::out);
			file << 0 << endl << 0 << endl << 0;
			file.close();
			return;
		}
		else {
			file >> record >> wintimes >> alltimes;
			file.close();
		}
	}
	//if there is record, save it
	else {
		if (win) wintimes++;
		else record = 0;
		alltimes++;
		file.open("2048log.log", ios_base::out);
		file << record << endl << wintimes << endl << alltimes;
		file.close();
	}
}

void savearch(){
	ofstream file("2048save", ios_base::out);
	for(int i = 0; i < width;i++)
		for(int j = 0; j < height; j++)
			file << s[i][j] << " ";
	file.close();
}
bool loadarch(){
	ifstream file("2048save", ios_base::in);
	if(!file)return false;
	else{
		cout << "Load the archive?y/n" << endl;
		if(cin.get() == 'y'){
			for(int i = 0; i < width; i++)
				for(int j = 0; j < height; j++)
					file >> s[i][j];
		file.close();
		remove("2048save");
		return true;
		}
		else return false;
	}
}
//interprete button number
int button2direction(int button) {
	if (button == 75)return left;
	else if (button == 72)return up;
	else if (button == 77)return right;
	else if (button == 80)return down;
	else return 0;
}

int main(){

	//initialize
	int direction = up, score = 0, record = 0, result = result_normal;
	float rate = 0;
	fileIO(record, true);
	changed = true;
	srand((int)time(0));

	if(!loadarch())generate();

	//initial success
	cout << "This is 2048 game, and you can control it by the direction key." << endl;
	cout << "Your pervious best score is " << record << ".  Want to break it? Press any key twice to begin!" << endl;
	_getch();_getch();
	print();
	
	while(1){
		if(_getch() == 27){
		    	cout << "Are you sure?y/n" << endl;
			if(cin.get() == 'y'){
				savearch();
				return 0;
			}
		}
		int button = _getch();
		
		direction = button2direction(button);
		if (direction == 0)continue;
		gravity(direction);
		popall(direction);
		gravity(direction);
		score++;

		result = judge();		
		if (result == result_normal) { 
			generate(); print();
		}
		else if (result == result_win) {
			cout << "you win! (^,^)" << endl << "your score is " << score << endl;
			fileIO(score, true);
			break;
		}
		else if (result == result_lose) {
			cout << "game over! :-(" << endl;
			fileIO(score, false);
		    break;
		}
		
	}
	rate = (float)wintimes / alltimes;
	cout << "Played game: " << alltimes << " ,winning rate: " << rate << endl;
	cout << "press any key to exit...";
	_getch();
	return 1;
}

