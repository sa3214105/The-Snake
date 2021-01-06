#include "model.h"
#define DEBUG
#define SEED 99999
#include <random>
int random(std::pair<int,int> range);
map::map(int _width,int _height,int _pix_size){
	srand(SEED);
	width=_width;
	height = _height;
	pix_size=_pix_size;
	game_snake=std::make_unique<snake>(this,pix_size);
	game_food=std::make_unique<food>(this,pix_size);
	for(int i=0;i<width;i++){
		obstacle.push_back({-1,i});
		obstacle.push_back({height,i});
	}
	for(int i=0;i<height;i++){
		obstacle.push_back({i,-1,});
		obstacle.push_back({i,width});
	}
	/*area.resize(height);
	std::for_each(area.begin(),area.end(),
				[&](decltype(*area.begin()) &vector_buf){
					vector_buf.resize(width);
				});*/
}
bool map::collisonCheck(std::pair<int,int> position){
	if(game_snake->collisonCheck(position)){
		return true;
	}
	if(std::find(obstacle.begin(),obstacle.end(),position)!=obstacle.end()){
		return true;
	}
	return false;
}
bool map::collisonCheck(std::pair<int,int> position,int layer){//未完成
	if(game_snake->collisonCheck(position,layer)){
		return true;
	}
	if(std::find(obstacle.begin(),obstacle.end(),position)!=obstacle.end()){
		return true;
	}
	return false;
}
bool map::collisonCheck(std::pair<int,int> position,int layer,std::vector<std::pair<int,int>> c_snake){//未完成
	//if(layer>c_snake.size()-1)layer=c_snake.size()-1;
	//auto end=c_snake.end()-1;
	if(std::find(c_snake.begin(),c_snake.end()-1,position)!=c_snake.end()-1){
		return true;
	}
	if(std::find(obstacle.begin(),obstacle.end(),position)!=obstacle.end()){
		return true;
	}
	return false;
}
bool map::complete(){

	return false;
}
void map::show(QPainter &painter){
	QPen pen;
	pen.setWidth(5);
	painter.setPen(pen);
	painter.drawRect(-5,-5,height*pix_size+5,width*pix_size+5);
	#ifdef DEBUG
		pen.setWidth(1);
		painter.setPen(pen);
		for(int i=0;i<width;i++){
			painter.drawLine(0,i*pix_size,pix_size*height,i*pix_size);
		}
		for(int i=0;i<height;i++){
			painter.drawLine(i*pix_size,0,i*pix_size,pix_size*width);
		}
	#endif  // DEBUG
	if(gamefail){
		painter.drawText(10, 220, "gameover");
	}
	game_snake->show(painter);
	game_food->show(painter);
	return;
}
void map::s_chage_direction(char D){
	game_snake->setDirection(D);
}
bool map::trigger(){
	game_snake->move();
	if(game_food->collisonCheck(game_snake->get_head())){
		game_snake->grow(true,gamefail);
	}else{
		game_snake->grow(false,gamefail);
	}
	
	if(gamefail==true){
		
		return false;
	}else{
		
		return true;
	}
}
std::pair<int,int> map::getHead(){
	return game_snake->get_head();
}
std::vector<std::pair<int,int>> map::getBody(){
	return game_snake->getBody();
}
std::pair<int,int> map::getFood(){
	return game_food->getFood();
}

food::food(map *_map,int _pix_size=10):pix_size(_pix_size){
	Fmap=_map;
	next_food();
}
void food::next_food(){
	if((Fmap->getBody().size()+1)>=(Fmap->getWidth()*Fmap->getHeight())-1){
		return;
	}
	int x=random({0,Fmap->getHeight()-1});
	int y=random({0,Fmap->getWidth()-1});
	//int x=rand()%Fmap->getHeight();
	//int y=rand()%Fmap->getWidth();
	//decltype(position) buf={x,y};
	position={x,y};
	if(Fmap->collisonCheck(position)==true){
		next_food();
	}
	auto xx=Fmap->getBody();
	auto tail=*(Fmap->getBody().end()-1);
	if((Fmap->getBody().size()+1)>=Fmap->getWidth()*Fmap->getHeight()-3){
		return;
	}
	if(manhattanDistance(position,tail)<1.42){
		next_food();
	}
	//position=buf;
	return;
}
double food::manhattanDistance(std::pair<int,int> start,std::pair<int,int> goal){
	//return std::abs(start.first-goal.first)+std::abs(start.second-goal.second);
	double a=std::abs(start.first-goal.first);
	double b=std::abs(start.second-goal.second);
	double c=std::pow(a,2)+std::pow(b,2);
	return std::pow(c,0.5);
}
bool food::collisonCheck(std::pair<int,int> _position){
	if(position==_position){
		next_food();
		return true;
	}
	return false;
}
void food::show(QPainter &painter){
	QPen pen;
	pen.setWidth(0);
	painter.setPen(pen);
	painter.setBrush(QBrush(Qt::red));
	painter.drawEllipse(position.first*pix_size,position.second*pix_size,pix_size,pix_size);
}
std::pair<int,int> food::getFood(){
	return position;
}

snake::snake(map *_map,int _pix_size):pix_size(_pix_size){
	Fmap=_map;
	direction='U';
	//init head
	head={3,3};
	body.resize(length);
	//init body
	auto head_copy=head;
	for(auto &block:body){
		++head_copy.second;
		block=head_copy;
	}
	nextStep={3,3};
}
void snake::show(QPainter &painter){
	//show head
	QPen pen;
	pen.setWidth(0);
	painter.setPen(pen);
	painter.setBrush(QBrush(Qt::blue));
	int a=head.first;
	painter.drawRect(head.first*pix_size,head.second*pix_size,pix_size,pix_size);
	//show body
	painter.setPen(pen);
	int xx=255;
	for(auto &block:body){
		painter.setBrush(QBrush(QColor(0,xx,0)));
		xx-=3;
		painter.drawRect(block.first*pix_size,block.second*pix_size,pix_size,pix_size);
	}
}
bool snake::collisonCheck(std::pair<int,int> position){
	if(head==position)return true;
	if(std::find(body.begin(),body.end()-1,position)!=body.end()-1){
		return true;
	}
	return false;
}
bool snake::collisonCheck(std::pair<int,int> position,int layer){
	//layer+=1;
	if(layer>length)layer=length;
	if(layer-1>=0)layer-=1;
	
	if(head==position)return true;
	if(std::find(body.begin(),body.end()-layer,position)!=body.end()-layer){
		return true;
	}
	return false;
}
void snake::move(){
	switch(direction){
		case 'U':
			nextStep.second--;
			break;
		case 'D':
			nextStep.second++;
			break;
		case 'L':
			nextStep.first--;
			break;
		case 'R':
			nextStep.first++;
			break;
	}
	if(std::find(body.begin(),body.end()-1,nextStep)!=body.end()-1){
		Fmap->gamefail=true;
		return;
	}
	if(std::find(Fmap->obstacle.begin(),Fmap->obstacle.end(),nextStep)!=Fmap->obstacle.end()){
		Fmap->gamefail=true;
		return;
	}
	body.insert(body.begin(),head);
	
	/*if(eatFood==false){
		
	}else{
		length++;
	}*/
	head=nextStep;
	
}
void snake::grow(bool food,bool gamefail){
	if(food){
		length++;
	}else{
		if(gamefail==false){
			body.pop_back();
		}
		
	}
}
auto snake::feasibleDirection()->std::set<char>{
	if(head.first<body[0].first){
		return {'U','D','L'};
	}else if(head.first>body[0].first){
		return {'U','D','R'};
	}else{
		if(head.second<body[0].second){
			return {'U','L','R'};
		}else if(head.second>body[0].second){
			return {'D','L','R'};
		}
	}
}
void snake::setDirection(char D){
	auto f_set=feasibleDirection();
	if (f_set.find(D)!=f_set.end()){//刪除不可行方向
		direction=D;
	}
}
std::pair<int,int> snake::get_head(){
	return head;
}
std::vector<std::pair<int,int>> snake::getBody(){
	return body;
}
int random(std::pair<int,int> range){
	std::random_device rd;
	auto gen = std::default_random_engine(rd());
	std::uniform_int_distribution<int> dis(range.first,range.second);
	return dis(gen);
}