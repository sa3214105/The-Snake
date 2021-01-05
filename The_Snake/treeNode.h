#include <vector>;
class treeNode {
	private:
		std::pair<int,int> position;//節點位置
		std::pair<int,int> goal;//節點位置
		char dire;//方向
		treeNode *superNode;//父節點
		std::vector<treeNode*> subNode;//子節點串列

	public:
		double weight;//權重
		treeNode(std::pair<int,int> _position,std::pair<int,int> _goal,char _dire,treeNode *_superNode);
		std::vector<std::pair<int,int>> getCurrSnake(int length);//取得虛擬蛇位置
		treeNode *getSuper();//取得父節點
		std::vector<treeNode*> getSub();//取得子節點
		double distance(std::pair<int,int> start,std::pair<int,int> goal);//計算距離
		std::vector<char> getDireList();//輸出方向串列
		void addNode(treeNode *N);//新增子節點
		std::pair<int,int> getPosition();//取得位置
		~treeNode();
};