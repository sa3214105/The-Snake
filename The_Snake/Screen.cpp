#include "Screen.h"
#define ss
#include <vector>
Screen::Screen(QWidget *parent):QWidget(parent){
	alg=std::make_unique<algorithm>(this);
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &Screen::uppdate);
	uppdate();
	timer->start(gameSpeed);
}
Screen::~Screen(){
	delete timer;
}
void Screen::paintEvent(QPaintEvent *event){
	QPainter painter(this);
	// 設定筆觸為SolidLine
	map_dp->show(painter);
}
void Screen::uppdate(){
	if(automatic){
		auto dire_buf=alg->nextDirection();
		if(dire_buf!='N'){
			dire=dire_buf;
		}else{
			alg->method(map_dp->getHead(),map_dp->getFood());
			dire=alg->nextDirection();
		}
	}
	map_dp->s_chage_direction(dire);
	if(map_dp->trigger()==false){
		timer->stop();
	}
	update();
}
void Screen::keyPressEvent(QKeyEvent* event){
	if(automatic==false){
		switch (event->key()) {
			case Qt::Key_Up:
				dire='U';
				break;
			case Qt::Key_Down:
				dire='D';
				break;
			case Qt::Key_Left:
				dire='L';
				break;
			case Qt::Key_Right:
				dire='R';
				break;
		}
	}
}
void Screen::reset(){
	timer->start(gameSpeed);
	map_dp=nullptr;
	map_dp=std::make_unique<map>(width,height,size);
	alg->outputQueue.clear();
	dire='U';
}
void Screen::pause(){
	timer->stop();
}
void Screen::start(){
	if(map_dp->gamefail==false)timer->start(gameSpeed);
}
void Screen::speedUp(){
	if(gameSpeed>=100)gameSpeed*=0.8;
	emit speed_change(QString::number(gameSpeed));
}
void Screen::speedLow(){
	if(gameSpeed<=2000)gameSpeed*=1.2;
	emit speed_change(QString::number(gameSpeed));
}
QString Screen::getSpeed(){
	return QString::number(gameSpeed);
}
void Screen::setAuto(){automatic=true;}
void Screen::setManual(){automatic=false;}
std::vector<std::pair<std::pair<int,int>,char>> Screen::nextStep(std::pair<int,int> curr,int layer){//層數判斷尚未完成
	std::vector<std::pair<std::pair<int,int>,char>> next
					={{{curr.first+1,curr.second},'R'},
					{{curr.first,curr.second+1},'D'},
					{{curr.first-1,curr.second},'L'},
					{{curr.first,curr.second-1},'U'}};
	auto p=next.begin();
	while(p!=next.end()){
		if(map_dp->collisonCheck(p->first,layer)){
			p=next.erase(p);
		}else{
			++p;
		}
	}
	return next;
}
std::vector<std::pair<std::pair<int,int>,char>> Screen::nextStep(std::pair<int,int> curr,int layer,std::vector<std::pair<int,int>> c_snake){
	std::vector<std::pair<std::pair<int,int>,char>> next
					={{{curr.first+1,curr.second},'R'},
					{{curr.first,curr.second+1},'D'},
					{{curr.first-1,curr.second},'L'},
					{{curr.first,curr.second-1},'U'}};
	auto p=next.begin();
	c_snake.erase(c_snake.begin());
	while(p!=next.end()){
		if(map_dp->collisonCheck(p->first,layer,c_snake)){
			p=next.erase(p);
		}else{
			++p;
		}
	}
	return next;
}
int Screen::getSnake_length(){
	return map_dp->getBody().size()+1;
}
std::vector<std::pair<int,int>> Screen::getSnake(){
	auto buf=map_dp->getBody();
	buf.insert(buf.begin(),map_dp->getHead());
	return buf;
}
