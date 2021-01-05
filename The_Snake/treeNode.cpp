#include "treeNode.h"
treeNode::treeNode(std::pair<int,int> _position,std::pair<int,int> _goal,char _dire,treeNode *_superNode):position(_position),goal(_goal),dire(_dire),superNode(_superNode){
	//��l�ưѼ�(��m,���I,��V,���`�I)
	weight=distance(position,goal);
}
double treeNode::distance(std::pair<int,int> start,std::pair<int,int> goal){
	//return std::abs(start.first-goal.first)+std::abs(start.second-goal.second);
	double a=std::abs(start.first-goal.first);
	double b=std::abs(start.second-goal.second);
	double c=std::pow(a,2)+std::pow(b,2);
	return std::pow(c,0.5);
}
std::vector<std::pair<int,int>> treeNode::getCurrSnake(int length){
	treeNode *ptr=this;//�ۤv����m
	std::vector<std::pair<int,int>> result;
	while(length>0&&ptr!=nullptr){
		result.push_back(ptr->position);//�[�J�`�I
		//result.insert(result.begin(),ptr->position);
		ptr=ptr->superNode;//���W���X
		--length;
	}
	return result;
}
treeNode *treeNode::getSuper(){
	return superNode;
}
std::vector<treeNode*> treeNode::getSub(){
	return subNode;
}
std::vector<char> treeNode::getDireList(){
	treeNode *ptr=this;//�ۤv����m
	std::vector<char> result;
	while(ptr->dire!='N'){
		result.insert(result.begin(),ptr->dire);//�[�J�`�I#��V
		ptr=ptr->superNode;//���W���X
	}
	return result;
}
void treeNode::addNode(treeNode *N){
	subNode.push_back(N);
}
std::pair<int,int> treeNode::getPosition(){
	return position;
}
treeNode::~treeNode(){
	for(auto &n:subNode){
		delete n;
	}
}