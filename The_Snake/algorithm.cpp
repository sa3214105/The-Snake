#include "Screen.h"
#include <functional>
#include "treeNode.h"
#include<QDebug>

template <class T> struct less {
  bool operator() (const T& x, const T& y) const {return x.first>y.first;}
  typedef T first_argument_type;
  typedef T second_argument_type;
  typedef bool result_type;
};
template <class T> struct less_t {
  bool operator() (const T& x, const T& y) const {return x->weight>y->weight;}
  typedef T first_argument_type;
  typedef T second_argument_type;
  typedef bool result_type;
};
algorithm::algorithm(Screen *ss){
	Obj=ss;
}
void algorithm::method(std::pair<int,int> curr_position,std::pair<int,int> food){
	//auto ss=Obj->nextStep(curr_position,0);
	//if(ss.size()!=0)outputQueue={ss[random({0,ss.size()-1})].second};
	outputQueue=aStar2(curr_position,food);
	int x=0;
};
char algorithm::random_dire(std::pair<int,int> curr_position){
	auto ss=Obj->nextStep(curr_position,0);
	if(ss.size()!=0){
		return ss[random({0,ss.size()-1})].second;
	}else{
		return 'T';
	}
}
char algorithm::nextDirection(){
	if(outputQueue.size()!=0){
		auto dire= outputQueue.front();
		outputQueue.pop_front();
		return dire;
	}else{
		return 'N';
	}
}

std::list<char> algorithm::aStar(std::pair<int,int> start,std::pair<int,int> goal){//未完成
	using NODE=std::pair<std::pair<int,int>,char>;//(位置,方向)
	using PATH=std::pair<double,std::deque<NODE>>;//(權重,節點串列)
	std::priority_queue<PATH,std::vector<PATH>,less<PATH>> qqueue;//優先Queue
	
	//std::queue<PATH> qqueue;
	auto curr_position=start;
	NODE n0={start,'N'};
	PATH p0={};
	p0.second.push_back(n0);
	p0.first=0;
	qqueue.push(p0);
	PATH result;
	while(qqueue.size()!=0){
		if(qqueue.size()>4000)break;//防止搜尋過長
		bool find_goal=false;
		PATH path=qqueue.top();//輸出最短路徑
		int top=path.first;
		qqueue.pop();//pop
		int pathSize=path.second.size();
		NODE currNode=path.second.back();//最後節點
		auto pathLength=path.second.size();//路徑長度
		auto nextNodes=Obj->nextStep(currNode.first,pathLength);//下個可能節點集
		auto nodeIter=nextNodes.begin();//可能節點走訪之迭代器
		auto Check_path_begin=path.second.cbegin();//確認節點是否會壓到路徑的起始點
		auto snake_length=Obj->getSnake_length();//蛇的長度
		int check_length;
		if(path.second.size()<snake_length){
			check_length=path.second.size()-1;
		}else{
			check_length=snake_length;
		}
		int x=0;
		auto Check_path_end=Check_path_begin+check_length;//確認節點是否會壓到路徑的結束點
		while(nodeIter!=nextNodes.end()){
			auto check=[&](NODE i)->bool{//lambda for check
				if((*nodeIter).first==i.first){
					return true;
				}else{
					return false;
				}
			};
			if(std::find_if(path.second.cbegin(),path.second.cend()/*Check_path_end*/,check)!=path.second.cend()){//如果節點有在路徑中的話//錯誤
				nodeIter=nextNodes.erase(nodeIter);//刪除
			}else{
				auto buf=path;//複製原路徑
				buf.second.push_back(*nodeIter);//把新節點接到最後
				buf.first=path.second.size()+manhattanDistance(nodeIter->first,goal);//更新路徑長度 曼哈頓距離未加
				qqueue.push(buf);//新路徑加到最後
				if((nodeIter->first)==goal){//是否找到食物
					find_goal=true;
					result=buf;//儲存紀錄
				}
				++nodeIter;//推進迭代器
				int x=0;
			}
			if(find_goal){
				break;
			}
		}
		if(find_goal){
			break;
		}
	}
	std::list<char> output;
	for(auto &node:result.second){
		if(node.second!='N'){
			output.push_back(node.second);
		}
	}
	if(output.empty()){//漫遊
		output.push_back(stroll(Obj->getSnake(),goal));
	}
	return output;
}
std::list<char> algorithm::aStar2(std::pair<int,int> start,std::pair<int,int> goal){//未完成
	qInfo();
	qInfo()<<goal;
	std::priority_queue<treeNode*,std::vector<treeNode*>,less_t<treeNode*>> qqueue;//優先Queue
	treeNode n0(start,goal,'N',nullptr);
	qqueue.push(&n0);
	std::vector<char> result;
	auto fullSnake=[&](std::vector<std::pair<int,int>> _path,bool x=true)->std::vector<std::pair<int,int>>{
		if(Obj->getSnake_length()+1>_path.size()){
			int fillLength=Obj->getSnake_length()-_path.size();
			if(x)++fillLength;
			if(fillLength>0);
			auto buf=Obj->getSnake();
			_path.insert(_path.end(),buf.begin()+1,buf.begin()+fillLength+1);
		}
		return _path;
	};
	int counter=0;
	while(qqueue.size()!=0){
		++counter;
		if(counter>10000){
			break;//防止搜尋過長
		}	
		bool find_goal=false;
		treeNode *headNode=qqueue.top();//輸出最短路徑
		auto vSnake=headNode->getCurrSnake(Obj->getSnake_length()+1);//取得演算法中蛇的位置
		
		qqueue.pop();//pop
		auto nextNodes=Obj->nextStep(headNode->getPosition(),vSnake.size());//下個可能節點集
		auto snake_length=Obj->getSnake_length();//蛇的長度
		auto nodeIter=nextNodes.begin();//可能節點走訪之迭代器
		while(nodeIter!=nextNodes.end()){
			auto check=[&](std::pair<int,int> i)->bool{//lambda for check
				if(nodeIter->first==i){
					return true;
				}else{
					return false;
				}
			};
			int x=0;
			if(vSnake.size()==snake_length)x=1;
			if(std::find_if(vSnake.begin(),vSnake.end()-x,check)!=vSnake.end()-x){//如果節點有在路徑中的話//錯誤
				nodeIter=nextNodes.erase(nodeIter);//刪除
			}else{
				treeNode* buf=new treeNode(nodeIter->first,goal,nodeIter->second,headNode);//建立新的子節點
				//buf->weight+=headNode->weight;//設定是否加速 免加權重
				headNode->addNode(buf);//把新節點接到最後
				qqueue.push(buf);//新路徑加到最後
				if((nodeIter->first)==goal){//是否找到食物
					auto xxxxx=buf->getCurrSnake(Obj->getSnake_length()+1);
					//qInfo()<<"row "<<xxxxx;
					if(find_tail(fullSnake(buf->getCurrSnake(Obj->getSnake_length()+1)))){//短路徑後半段會有問題
						find_goal=true;
						result=buf->getDireList();//儲存紀錄
						food_buf=true;//紀錄有吃到食物
						qInfo()<<"row "<<fullSnake(buf->getCurrSnake(Obj->getSnake_length()+1));
						//for(auto &xx:vSnake)qInfo()<<xx;
					}
				}
				++nodeIter;//推進迭代器
				int x=0;
			}
			if(find_goal){
				qInfo()<<"a*";
				break;
			}
		}
		if(find_goal){
			break;
		}
	}
	
	std::list<char> output;
	for(auto &node:result){
		if(node!='N'){
			output.push_back(node);
		}
	}
	if(output.empty()){//漫遊
		output.push_back(stroll(Obj->getSnake(),goal));
		qInfo()<<"stroll";
	}
	return output;
}
int algorithm::random(std::pair<int,int> range){
	std::random_device rd;
	auto gen = std::default_random_engine(rd());
	std::uniform_int_distribution<int> dis(range.first,range.second);
	return dis(gen);
}
double algorithm::manhattanDistance(std::pair<int,int> start,std::pair<int,int> goal){
	//return std::abs(start.first-goal.first)+std::abs(start.second-goal.second);
	double a=std::abs(start.first-goal.first);
	double b=std::abs(start.second-goal.second);
	double c=std::pow(a,2)+std::pow(b,2);
	return std::pow(c,0.5);
}
char algorithm::stroll(std::vector<std::pair<int,int>> snake,std::pair<int,int> food){//漫遊 找尾未完成
	auto curr_position=*snake.begin();
	auto ss=Obj->nextStep(curr_position,1);//ss是下個步伐集
	qInfo()<<"dire";
	qInfo()<<ss;
	if(ss.size()!=0){
		double buf=0;//紀錄距離
		char output='N';//紀錄方向
		std::pair<int,int> position;//紀錄位置
		for(auto &node:ss){
			auto distance=manhattanDistance(node.first,food);
			if(distance>buf){
				position=node.first;
				auto testTail=[&]()->bool{
					snake.erase(snake.end()-1);
					snake.insert(snake.begin(),position);
					if(food_buf){//前一動有吃過東西 尾巴不可撞
						return find_tail(snake,true);
					}else{//前一動是漫遊 尾巴可撞
						return find_tail(snake,false);
					}
					
				};
				if(testTail()==true){
					buf=distance;
					output=node.second;
				}
			}
		}
		qInfo()<<"ddiirreee"<<output;
		food_buf=false;
		return output;
	}else{
		food_buf=false;
		qInfo()<<"TT";
		return 'T';
	}
}
bool algorithm::find_tail(std::vector<std::pair<int,int>> snake,bool grow){
	//qInfo()<<snake;
	std::priority_queue<treeNode*,std::vector<treeNode*>,less_t<treeNode*>> qqueue;//優先Queue
	auto goal=*(snake.end()-1);//蛇的尾巴是終點
	treeNode *n0=new treeNode(*snake.begin(),goal,'N',nullptr);//蛇的頭是搜尋起點
	qqueue.push(n0);
	std::vector<char> result;
	auto fullSnake=[&](std::vector<std::pair<int,int>> _path,bool x=true)->std::vector<std::pair<int,int>>{
		if(Obj->getSnake_length()>_path.size()){
			int fillLength=Obj->getSnake_length()-_path.size();
			if(x)++fillLength;
			if(fillLength>0);
			auto buf=snake;
			_path.insert(_path.end(),buf.begin(),buf.begin()+fillLength);
		}
		return _path;
	};
	if(grow){//防尾追撞 grow表上一動有吃東西
		if(manhattanDistance(*snake.begin(),*(snake.end()-1))<=1){
			auto head=snake.begin();
			bool xx=false;
			std::vector<std::pair<int,int>> nextnode={{head->first,head->second-1},
													  {head->first,head->second+1},
													  {head->first-1,head->second},
													  {head->first+1,head->second}};//-
			qInfo()<<"ss "<<nextnode;
			for(auto &node:nextnode){
				if(node.first<0||node.second<0){	
					break;
				}
				if(node.first>(Obj->width)||node.second>(Obj->height)){
					break;
				}
				if(std::find(snake.begin(),snake.end()-1,node)==snake.end()-1){//除了尾巴以外生體不能撞
					//虛擬snake合成
					/*auto tsnake=snake;
					tsnake.pop_back();
					tsnake.insert(tsnake.begin(),node);
					xx=find_tail(tsnake,false);*/
					break;
				}
				if(node==*(snake.end()-1)){
					//虛擬snake合成
					auto tsnake=snake;
					tsnake.pop_back();
					tsnake.insert(tsnake.begin(),node);
					xx=find_tail(tsnake,true);
					break;
				}
			}
			if(xx==false){
				return false;
			}
		}
	}
	
	while(qqueue.size()!=0){
		if(qqueue.size()>1000)break;//防止搜尋過長
		bool find_goal=false;
		treeNode *headNode=qqueue.top();//輸出最短路徑
		auto vSnake=headNode->getCurrSnake(Obj->getSnake_length());//取得演算法中蛇的位置
		auto ffss=fullSnake(vSnake);
		qqueue.pop();//pop
		auto nextNodes=Obj->nextStep(headNode->getPosition(),0,snake);//下個可能節點集
		auto snake_length=Obj->getSnake_length();//蛇的長度
		auto nodeIter=nextNodes.begin();//可能節點走訪之迭代器
		while(nodeIter!=nextNodes.end()){
			auto check=[&](std::pair<int,int> i)->bool{//lambda for check
				if(nodeIter->first==i){
					return true;
				}else{
					return false;
				}
			};
			if(std::find_if(vSnake.begin(),vSnake.end(),check)!=vSnake.end()){//如果節點有在路徑中的話//錯誤
				nodeIter=nextNodes.erase(nodeIter);//刪除
			}else{
				treeNode* buf=new treeNode(nodeIter->first,goal,nodeIter->second,headNode);//建立新的子節點
				//buf->weight+=headNode->weight;
				headNode->addNode(buf);//把新節點接到最後
				qqueue.push(buf);//新路徑加到最後
				if((nodeIter->first)==goal){//是否找到尾巴
					find_goal=true;
					result=buf->getDireList();//儲存紀錄
					//for(auto &xx:result)qInfo()<<xx;
				}
				++nodeIter;//推進迭代器
				int x=0;
			}
			if(find_goal){
				//break;
				return true;
			}
		}
		if(find_goal){
			//break;
		}
	}
	delete n0;
	return false;
}
