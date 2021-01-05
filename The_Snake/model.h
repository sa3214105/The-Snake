#pragma once
#ifndef TEST
	#define TEST
	#include <QApplication>
	#include <QPainter>
	#include <QWidget>
	#include <utility>
	#include <vector>
	#include <set>
	
#endif  // !test
#include <set>

class map;
class snake{
 private:
	std::pair<int, int> head;
	std::vector<std::pair<int, int>> body;
	std::pair<int,int> nextStep;
	char direction='U';
	int length=3;
	int pix_size;
	map *Fmap=nullptr;
 public:
	snake(map *_map,int _pix_size);
	void move();
	void setDirection(char D);
	void show(QPainter &);
	bool collisonCheck(std::pair<int,int> position);
	bool collisonCheck(std::pair<int,int> position,int layer);
	auto feasibleDirection()->std::set<char>;
	std::pair<int,int> get_head();
	std::vector<std::pair<int,int>> getBody();
	void grow(bool food,bool gamefail);
};
class food{
 private:
	std::pair<int,int> position;
	int pix_size;
	map *Fmap=nullptr;
 public:
	food(map *_map,int _pix_size);
	void next_food();
	bool collisonCheck(std::pair<int,int> _position);
	void show(QPainter &);
	std::pair<int,int> getFood();
	double food::manhattanDistance(std::pair<int,int> start,std::pair<int,int> goal);
};
class map{
 private:
	int width=20;
	int height=20;
	int pix_size=10;
	std::unique_ptr<snake> game_snake;
	std::unique_ptr<food> game_food;
 public:
	map(int _width,int _height,int _pix_size);
	bool collisonCheck(std::pair<int,int> position);//整條蛇碰撞
	bool collisonCheck(std::pair<int,int> position,int layer);//部分蛇碰撞
	bool collisonCheck(std::pair<int,int> position,int layer,std::vector<std::pair<int,int>> c_snake);//自訂蛇碰撞
	bool complete();
	void show(QPainter &);
	int getHeight(){return height;}
	int getWidth(){return width;}
	void s_chage_direction(char D);//改變方向
	bool trigger();
	bool gamefail=false;
	std::pair<int,int> getFood();
	std::vector<std::pair<int,int>> getBody();
	std::pair<int,int> getHead();
	std::vector<std::pair<int,int>> obstacle;//地圖障礙物
	
};


