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

std::list<char> algorithm::aStar(std::pair<int,int> start,std::pair<int,int> goal){//������
	using NODE=std::pair<std::pair<int,int>,char>;//(��m,��V)
	using PATH=std::pair<double,std::deque<NODE>>;//(�v��,�`�I��C)
	std::priority_queue<PATH,std::vector<PATH>,less<PATH>> qqueue;//�u��Queue
	
	//std::queue<PATH> qqueue;
	auto curr_position=start;
	NODE n0={start,'N'};
	PATH p0={};
	p0.second.push_back(n0);
	p0.first=0;
	qqueue.push(p0);
	PATH result;
	while(qqueue.size()!=0){
		if(qqueue.size()>4000)break;//����j�M�L��
		bool find_goal=false;
		PATH path=qqueue.top();//��X�̵u���|
		int top=path.first;
		qqueue.pop();//pop
		int pathSize=path.second.size();
		NODE currNode=path.second.back();//�̫�`�I
		auto pathLength=path.second.size();//���|����
		auto nextNodes=Obj->nextStep(currNode.first,pathLength);//�U�ӥi��`�I��
		auto nodeIter=nextNodes.begin();//�i��`�I���X�����N��
		auto Check_path_begin=path.second.cbegin();//�T�{�`�I�O�_�|������|���_�l�I
		auto snake_length=Obj->getSnake_length();//�D������
		int check_length;
		if(path.second.size()<snake_length){
			check_length=path.second.size()-1;
		}else{
			check_length=snake_length;
		}
		int x=0;
		auto Check_path_end=Check_path_begin+check_length;//�T�{�`�I�O�_�|������|�������I
		while(nodeIter!=nextNodes.end()){
			auto check=[&](NODE i)->bool{//lambda for check
				if((*nodeIter).first==i.first){
					return true;
				}else{
					return false;
				}
			};
			if(std::find_if(path.second.cbegin(),path.second.cend()/*Check_path_end*/,check)!=path.second.cend()){//�p�G�`�I���b���|������//���~
				nodeIter=nextNodes.erase(nodeIter);//�R��
			}else{
				auto buf=path;//�ƻs����|
				buf.second.push_back(*nodeIter);//��s�`�I����̫�
				buf.first=path.second.size()+manhattanDistance(nodeIter->first,goal);//��s���|���� �ҫ��y�Z�����[
				qqueue.push(buf);//�s���|�[��̫�
				if((nodeIter->first)==goal){//�O�_��쭹��
					find_goal=true;
					result=buf;//�x�s����
				}
				++nodeIter;//���i���N��
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
	if(output.empty()){//���C
		output.push_back(stroll(Obj->getSnake(),goal));
	}
	return output;
}
std::list<char> algorithm::aStar2(std::pair<int,int> start,std::pair<int,int> goal){//������
	qInfo();
	qInfo()<<goal;
	std::priority_queue<treeNode*,std::vector<treeNode*>,less_t<treeNode*>> qqueue;//�u��Queue
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
			break;//����j�M�L��
		}	
		bool find_goal=false;
		treeNode *headNode=qqueue.top();//��X�̵u���|
		auto vSnake=headNode->getCurrSnake(Obj->getSnake_length()+1);//���o�t��k���D����m
		
		qqueue.pop();//pop
		auto nextNodes=Obj->nextStep(headNode->getPosition(),vSnake.size());//�U�ӥi��`�I��
		auto snake_length=Obj->getSnake_length();//�D������
		auto nodeIter=nextNodes.begin();//�i��`�I���X�����N��
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
			if(std::find_if(vSnake.begin(),vSnake.end()-x,check)!=vSnake.end()-x){//�p�G�`�I���b���|������//���~
				nodeIter=nextNodes.erase(nodeIter);//�R��
			}else{
				treeNode* buf=new treeNode(nodeIter->first,goal,nodeIter->second,headNode);//�إ߷s���l�`�I
				//buf->weight+=headNode->weight;//�]�w�O�_�[�t �K�[�v��
				headNode->addNode(buf);//��s�`�I����̫�
				qqueue.push(buf);//�s���|�[��̫�
				if((nodeIter->first)==goal){//�O�_��쭹��
					auto xxxxx=buf->getCurrSnake(Obj->getSnake_length()+1);
					//qInfo()<<"row "<<xxxxx;
					if(find_tail(fullSnake(buf->getCurrSnake(Obj->getSnake_length()+1)))){//�u���|��b�q�|�����D
						find_goal=true;
						result=buf->getDireList();//�x�s����
						food_buf=true;//�������Y�쭹��
						qInfo()<<"row "<<fullSnake(buf->getCurrSnake(Obj->getSnake_length()+1));
						//for(auto &xx:vSnake)qInfo()<<xx;
					}
				}
				++nodeIter;//���i���N��
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
	if(output.empty()){//���C
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
char algorithm::stroll(std::vector<std::pair<int,int>> snake,std::pair<int,int> food){//���C ���������
	auto curr_position=*snake.begin();
	auto ss=Obj->nextStep(curr_position,1);//ss�O�U�ӨB�ﶰ
	qInfo()<<"dire";
	qInfo()<<ss;
	if(ss.size()!=0){
		double buf=0;//�����Z��
		char output='N';//������V
		std::pair<int,int> position;//������m
		for(auto &node:ss){
			auto distance=manhattanDistance(node.first,food);
			if(distance>buf){
				position=node.first;
				auto testTail=[&]()->bool{
					snake.erase(snake.end()-1);
					snake.insert(snake.begin(),position);
					if(food_buf){//�e�@�ʦ��Y�L�F�� ���ڤ��i��
						return find_tail(snake,true);
					}else{//�e�@�ʬO���C ���ڥi��
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
	std::priority_queue<treeNode*,std::vector<treeNode*>,less_t<treeNode*>> qqueue;//�u��Queue
	auto goal=*(snake.end()-1);//�D�����ڬO���I
	treeNode *n0=new treeNode(*snake.begin(),goal,'N',nullptr);//�D���Y�O�j�M�_�I
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
	if(grow){//�����l�� grow��W�@�ʦ��Y�F��
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
				if(std::find(snake.begin(),snake.end()-1,node)==snake.end()-1){//���F���ڥH�~���餣�༲
					//����snake�X��
					/*auto tsnake=snake;
					tsnake.pop_back();
					tsnake.insert(tsnake.begin(),node);
					xx=find_tail(tsnake,false);*/
					break;
				}
				if(node==*(snake.end()-1)){
					//����snake�X��
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
		if(qqueue.size()>1000)break;//����j�M�L��
		bool find_goal=false;
		treeNode *headNode=qqueue.top();//��X�̵u���|
		auto vSnake=headNode->getCurrSnake(Obj->getSnake_length());//���o�t��k���D����m
		auto ffss=fullSnake(vSnake);
		qqueue.pop();//pop
		auto nextNodes=Obj->nextStep(headNode->getPosition(),0,snake);//�U�ӥi��`�I��
		auto snake_length=Obj->getSnake_length();//�D������
		auto nodeIter=nextNodes.begin();//�i��`�I���X�����N��
		while(nodeIter!=nextNodes.end()){
			auto check=[&](std::pair<int,int> i)->bool{//lambda for check
				if(nodeIter->first==i){
					return true;
				}else{
					return false;
				}
			};
			if(std::find_if(vSnake.begin(),vSnake.end(),check)!=vSnake.end()){//�p�G�`�I���b���|������//���~
				nodeIter=nextNodes.erase(nodeIter);//�R��
			}else{
				treeNode* buf=new treeNode(nodeIter->first,goal,nodeIter->second,headNode);//�إ߷s���l�`�I
				//buf->weight+=headNode->weight;
				headNode->addNode(buf);//��s�`�I����̫�
				qqueue.push(buf);//�s���|�[��̫�
				if((nodeIter->first)==goal){//�O�_������
					find_goal=true;
					result=buf->getDireList();//�x�s����
					//for(auto &xx:result)qInfo()<<xx;
				}
				++nodeIter;//���i���N��
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
