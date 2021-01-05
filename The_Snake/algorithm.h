#include <list>
#include <utility>
#include <random>
#include <queue>
#ifndef ALG
#define ALG
class Screen;
class algorithm{
	private:
		Screen *Obj;
	protected:
		std::list<char> aStar(std::pair<int,int> start,std::pair<int,int> goal);
		std::list<char> aStar2(std::pair<int,int> start,std::pair<int,int> goal);
	public:
		std::list<char> outputQueue;
		algorithm(Screen *ss);
		void method(std::pair<int,int> curr_position,std::pair<int,int> food);
		char nextDirection();
		int random(std::pair<int,int> range);
		double manhattanDistance(std::pair<int,int> start,std::pair<int,int> goal);
		char random_dire(std::pair<int,int> curr_position);
		char stroll(std::vector<std::pair<int,int>> snake,std::pair<int,int> food);
		bool algorithm::find_tail(std::vector<std::pair<int,int>> snake,bool grow=true);
		bool food_buf=true;
		//bool algorithm::check_goal(std::pair<int,int> position,std::pair<int,int> snake_tail);
};
/**/
#endif ALG