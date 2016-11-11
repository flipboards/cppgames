#include"mine.h"

//Lattice Functions
lattice::lattice(){
	ismine = 0;
	state = STATE_CLOSED;
	label = 0;
}
void lattice::setmine(){
	ismine = 1;
}
void lattice::setneighbour(vector<lattice*> neighbour){
	this->neighbours = neighbour;
}
void lattice::setlabel(){
	for each(lattice* nb in neighbours){
		label += (int)nb->ismine;
	}
}
bool lattice::open(){
	if(ismine)this->state = STATE_KILLED;
	else this->state = STATE_OPENED;
	return ismine;
}
int lattice::getlabel()const{
	if(this->state == STATE_OPENED)return this->label;
	else return -1;
}

//Minemap functions
mineMap::mineMap(){
	m_lose = 0;
	m_lattices = 0;
}
mineMap::mineMap(int width,int height){
	m_width = width;
	m_height = height;
	m_lose = 0;
	m_lattices = 0;
}
mineMap::~mineMap(){
	delete m_lattices;
}

lattice* mineMap::getLattice(int x,int y)const{
	return m_lattices + x + y*m_width;
}
vector<lattice*> mineMap::setNeighbour(lattice* me){

	int index = me - m_lattices;
	vector<lattice*> neighbour;

	int right = Min(index %m_width +1,m_width-1);
	int bottom = Min(index /m_width +1,m_height-1);
	for(int i = Max(index %m_width -1, 0); i <= right; i++){
		for(int j= Max(index /m_width -1, 0);j <= bottom; j++){
			neighbour.push_back(getLattice(i,j));
		}
	}
	return neighbour;
}
int mineMap::Open(lattice* Lattice){

	if(Lattice->state != STATE_CLOSED)return 0;
	if(Lattice->open()){m_lose=1;return 0;}		//gameover!
	
	if(Lattice->getlabel() == 0){

		for each(lattice* neighbour in Lattice->neighbours){
			if(neighbour->state == STATE_CLOSED){this->Open(neighbour);this->Draw();}
		}
	}
	m_opennumber++;
	return 1;
}
int mineMap::FastOpen(lattice* Lattice){
	if(Lattice->state!=STATE_OPENED)return 0;

	int count=0;//实际雷数
	for each(lattice* neighbour in Lattice->neighbours){
		if(neighbour->state==STATE_MARKED)count++;	//计算周围没开过雷的个数
	}
	if(count != Lattice->getlabel())return 0;			//判断要开的雷数目是否等于标记的个数
	
	for each(lattice* neighbour in Lattice->neighbours){
		if(neighbour->state==STATE_CLOSED)this->Open(neighbour);
	}
	return 1;
}
int mineMap::Mark(lattice* Lattice){
	if(Lattice->state==STATE_CLOSED){Lattice->state=STATE_MARKED;m_mineleft--;}
	else if(Lattice->state==STATE_MARKED){Lattice->state=STATE_CLOSED;m_mineleft++;}
	
	return 1;
}

//mineMap public functions
int mineMap::Generate(int width,int height,int mineNumber,int seedX,int seedY){

	if(mineNumber>width*height-1)return 0;

	m_width = width;
	m_height = height;
	m_mineleft = mineNumber;
	m_opennumber = 0;
	m_minenumber = mineNumber;
	m_lose=0;
	
	//generate map
	m_lattices=new lattice[width*height];
	for(int i=0;i<width*height;i++)
		m_lattices[i].setneighbour(setNeighbour(m_lattices + i));

	//generate mines	
generate:	
	int* mines = randomChoose(m_width*m_height,mineNumber);
	
	for each(lattice* nb in getLattice(seedX,seedY)->neighbours){
		if(find(mines,mines + mineNumber,nb - m_lattices)!=( mines + mineNumber))goto generate;
	}

	for(int i=0;i<mineNumber;i++)
		m_lattices[mines[i]].setmine();
	delete mines;

	//calculate the label of each lattice
	for(int i = 0; i <width*height ; i++)
		m_lattices[i].setlabel();
	return 1;
}
int mineMap::Open(int x,int y){
	//cout<<"x="<<x<<", y="<<y<<" state="<<getLattice(x,y)->state<<" label="<<getLattice(x,y)->label<<endl;
	return this->Open(getLattice(x,y));
}
int mineMap::Mark(int x,int y){
	return this->Mark(getLattice(x,y));
}
int mineMap::FastOpen(int x,int y){
	return this->FastOpen(getLattice(x,y));
	
}
int mineMap::GameState()const{
	if(m_lose)return M_LOSE;
	else if(m_minenumber == m_width*m_height - m_opennumber)return M_WIN;
	else return M_NORMAL;
}
void mineMap::Draw()const{
	if(m_drawer)m_drawer->Draw();
}

//mapDrawer functions
Scalar mapDrawer::getColor(int num,int flag){
	if(flag == STATE_OPENED){
		if(num == 1)return Scalar(255,128,0);
		else if(num == 2)return Scalar(0,255,128);
		else if(num == 3)return Scalar(0,255,255);
		else if(num == 4)return Scalar(0,188,249);
		else if(num == 5)return Scalar(255,0,255);
		else if(num == 6)return Scalar(191,0,191);
		else if(num == 7)return Scalar(128,0,255);
		else if(num == 8)return Scalar(0,0,255);
		else return Scalar(0,0,0);
	}
	else if(flag == STATE_CLOSED){
		return Scalar(13,219,249);
	}
	else if(flag == STATE_KILLED){
		return Scalar(28,11,232);
	}
	else if(flag == STATE_MARKED){
		return cvRed;
	}
	else return Scalar(0,0,0);
}
void mapDrawer::paint(Mat& img,Point point,int lengthx,int lengthy,const Scalar &color,int flag){
	if(flag == DRAW_NORMAL)rectangle(img,point,Point(point.x+lengthx,point.y+lengthy),color,-1);
	else if(flag == DRAW_ROUND)circle(img,Point(point.x+0.5*lengthx,point.y+0.5*lengthy),Min(lengthx/2,lengthy/2),color,-1);
}
void mapDrawer::Bind(mineMap* map,const string window,int width,int height,int drawflag,int label){
	this->map = map;
	this->window = window;
	this->width = width;
	this->height = height;
	this->drawflag = drawflag;
	this->drawLabel = label;
}
void mapDrawer::Draw(){

	Mat pic(height,width,CV_8UC3,cvBlack);
	const int cellW = width/map->m_width,cellH = height/map->m_height;

	if(!map->m_lattices){
		pic = Mat(height,width,CV_8UC3,getColor(0,STATE_CLOSED));
		for(int x=1;x<map->m_width;x++){
			line(pic,Point(x*cellW,0),Point(x*cellW,height),cvWhite);
		}
		for(int y=1;y<map->m_height;y++){
			line(pic,Point(0,y*cellH),Point(width,y*cellH),cvWhite);
		}
		imshow(window,pic);
		return;
	}

	for(int x=0;x<map->m_width;x++)
		for(int y=0;y<map->m_height;y++){
			if(map->getLattice(x,y)->state != STATE_OPENED){
				if(drawflag != DRAW_DARK)
					paint(pic,Point(cellW*x,cellH*y),cellW,cellH,getColor(0,map->getLattice(x,y)->state),drawflag);
				else{
					if(map->getLattice(x,y)->state == STATE_MARKED||map->getLattice(x,y)->state == STATE_KILLED)
						paint(pic,Point(cellW*x,cellH*y),cellW,cellH,getColor(0,map->getLattice(x,y)->state),0);
				}
				if(drawflag == DRAW_NORMAL){
					rectangle(pic,Point(cellW*x,cellH*y),Point(cellW*(x+1),cellH*(y+1)),cvWhite,1);
				}
				
			}
			else{
				if(drawLabel){
					if(map->getLattice(x,y)->getlabel()>0)
						putText(pic,toString(map->getLattice(x,y)->getlabel()),Point(cellW*(x+0.2),cellH*(y+0.8)),CV_FONT_HERSHEY_DUPLEX,0.8,
					getColor(map->getLattice(x,y)->getlabel(),STATE_OPENED));
				}
				else{
					paint(pic,Point(cellW*x,cellH*y),cellW,cellH,getColor(map->getLattice(x,y)->getlabel(),STATE_OPENED),0);
				}

			}
		}
	imshow(window,pic);
}

//solveMap Functions
int solveMap::Bind(mineMap* map){
	if(map ==0)return 0;
	else {
		m_map = map;
		mapsize = map->m_width*map->m_height;
		mineleft = new int[mapsize];
		nbleft = new int[mapsize];
		nbopen = new int[mapsize];
		for(int i = 0; i < mapsize; i++){
			mineleft[i] = 8;
			nbleft[i] = 8;
			nbopen[i] = 0;
		}
	}
	return 1;
}
solveMap::~solveMap(){
	delete mineleft;
	delete nbleft;
	delete nbopen;
}

//扫描基本数据，决定是否需要打开
void solveMap::scan()	//复杂度O(n)*nb
{
	for(int n = 0;n < mapsize; n++){
		//筛选出之前未处理的，打开的格子
		if(nbleft[n] > 0){
			
			mineleft[n] = m_map->m_lattices[n].getlabel();
			nbleft[n] = 0;
			nbopen[n] = 0;

			if (m_map->m_lattices[n].state == STATE_MARKED)continue;

			for each(lattice* neighbour in m_map->m_lattices[n].neighbours){
				if(neighbour->state == STATE_CLOSED)nbleft[n]++;
				else if(neighbour->state == STATE_MARKED)mineleft[n]--;
				else nbopen[n]++;
			}
		}
	}
	return;
}
//把显而易见的格子打开
int solveMap::justOpen()	//复杂度O(n)
{
	int opennumber = 0;

	for (int n = 0; n < mapsize; n++) {

		if (nbleft[n] > 0 && m_map->m_lattices[n].state == STATE_OPENED){	//only opened lattices are valued for test
			
			if (mineleft[n] == nbleft[n]) {		//if just left mine[n] neighbours, mark all of them
				for each(lattice* neighbour in m_map->m_lattices[n].neighbours) {
					if (neighbour->state == STATE_CLOSED)m_map->Mark(neighbour);
				}
				opennumber++;
			}
			else if (mineleft[n] == 0) {		//open fully marked lattices
				m_map->FastOpen(m_map->m_lattices + n);
				opennumber++;
			}
		}
	}
	return opennumber;
}
//局部概率计算，如1-2-1这种	(DFS)
int solveMap::getnbdetail(int index,vector<lattice*>& closednb,vector<lattice*>& openednb){

	for each(lattice* neighbour in m_map->m_lattices[index].neighbours){
		if(neighbour->state == STATE_CLOSED) closednb.push_back(neighbour);
		for each(lattice* neighbour2 in neighbour->neighbours){
			if(nbleft[neighbour2 - m_map->m_lattices]>0 && find(openednb.begin(), openednb.end(), neighbour2) == openednb.end())
				openednb.push_back(neighbour2);
		}
	}
	return openednb.size();
}
int solveMap::microProb(){	//复杂度O(n)*2^nb

	int solvenumber = 0;
	for(int n = 0;n < mapsize; n++){

		if(nbleft[n] >0 && m_map->m_lattices[n].state == STATE_OPENED){

			vector<lattice*> openednb, closednb;			//开过的格子和没开的格子,都是lattice*
			if(!getnbdetail(n,closednb,openednb))continue;

			//生成相关性矩阵 get common neighbours
			matrix covList(openednb.size(),closednb.size());
			vector<symbol> symbols(openednb.size(),le);
			
			for(unsigned int i = 0; i < openednb.size(); i++){
				int containmark = 0;
				for(unsigned int j = 0; j < closednb.size(); j++){
					//find if closednb is in openednb[i]'s neighbour
					if(find(openednb[i]->neighbours.begin(),openednb[i]->neighbours.end(),closednb[j]) != openednb[i]->neighbours.end()){
						covList.data[i][j] = 1;
						containmark ++;
					}
					else{
						covList.data[i][j] = 0;
					}
				}
				if(containmark == nbleft[openednb[i] - m_map->m_lattices]) symbols[i] = symbol::equal;
			}

			//解不定方程组
			vector<int> result(closednb.size(),0);
			vector<int> restrict;
			for(unsigned int i = 0; i < openednb.size(); i++){
				restrict.push_back(mineleft[openednb[i] - m_map->m_lattices]);
			}

			int cnt_result = intProgram(covList,restrict,result,symbols);
			
			//激动人心的时刻……
			for(unsigned int i=0; i < result.size(); i++){
				if(result[i] == cnt_result){		//所有情况都为1，标记
					m_map->Mark(closednb[i]);
					solvenumber ++;
				}
				else if(result[i] == 0){			//所有情况都没有，打开
					m_map->Open(closednb[i]);
					solvenumber ++;
				}
			}
		}
//		cout<<n<<endl;
	}
	return solvenumber;
}
//片状概率计算，并生成概率矩阵
int solveMap::pieceDetect(lattice* me, vector<lattice*>& closedpc, vector<lattice*>& openedpc, matrix& matcov, vector<bool>& record)
{
	
	vector<int> covline(closedpc.size(),0);		//准备放到矩阵中的一行

	bool includemark = false;
	for each(lattice* nb in me->neighbours) {
		if (nb->state == STATE_CLOSED) {
			vector<lattice*>::iterator it_nb = find(closedpc.begin(), closedpc.end(), nb);

			if (it_nb == closedpc.end()) {		//找不到，放进去
				closedpc.push_back(nb);
				covline.push_back(1);
			}
			else {								//找到了，标记
				covline[it_nb - closedpc.begin] = 1;
				includemark = true;
			}
		}
		else if (nb!= me && nb->getlabel() > 0) {
			if(find(openedpc.begin(),openedpc.end(),nb)==openedpc.end()){
				pieceDetect(nb, closedpc, openedpc, matcov);
			}
		}
	}

	//要是符合条件就放入矩阵
	if (includemark) {
		openedpc.push_back(me);
		matcov.addline(covline);
		record[me - m_map->m_lattices] = true;
	}
	return openedpc.size();
}
int solveMap::pieceProb()
{
	vector<bool> record(mapsize, false);
	for (int n = 0; n < mapsize; n++) {
		
		matrix cov(1,1);
		if (!record[n] && m_map->m_lattices[n].getlabel() > 0) {

			vector<lattice*> closedpiece, openedpiece;
			pieceDetect(m_map->m_lattices + n, closedpiece, openedpiece, cov, record);

			//生成限制向量
			vector<int> restrict;
			for (vector<lattice*>::iterator op = openedpiece.begin; op<openedpiece.end();op++) {
				restrict.push_back(mineleft[*op - m_map->m_lattices]);
			}

			//计算概率
			vector<float> result;
			floatProgram(cov, restrict, result);

			//概率实现
			for (int i = 0; i < result.size(); i++) {
				P[closedpiece[i] - m_map->m_lattices] = result[i];
			}
		}
	}
	return 1;
}
//根据剩下雷多少判断概率
int solveMap::finalDetect(){	//复杂度O(n)
	
	int opennumber = 0;

	int relative_cnt = 0;			//有关的
	vector<lattice*> remain;		//剩下的
	for (int n = 0; n < mapsize; n++) {
		if (m_map->m_lattices[n].state == STATE_CLOSED) {
			if (nbopen[n] > 0)relative_cnt++;
			else {
				remain.push_back(m_map->m_lattices + n);
			}
		}
	}
	for each(lattice* l in remain) {
		P[l - m_map->m_lattices] = (m_map->m_mineleft - relative_cnt) / remain.size();
	}

	if (m_map->m_mineleft == relative_cnt) {
		for each(lattice* l in remain) {
			m_map->Open(l);
		}
		return opennumber;
	}
	else
		return 0;
}
//BFS
int solveMap::ProbExecute()
{
	for (unsigned int n = 0; n < mapsize; n++) {
		if (P[n] == 0) {
			m_map->FastOpen(m_map->m_lattices + n);
		}
		else if (P[n] == 1) {
			m_map->Mark(m_map->m_lattices + n);
		}
	}
	return 1;
}
int solveMap::Solve(int option = 0){
	
	scan();
	while (justOpen() || microProb()) {
		scan();
	}
	finalDetect();
	return 1;
}
