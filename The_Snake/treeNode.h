#include <vector>;
class treeNode {
	private:
		std::pair<int,int> position;//�`�I��m
		std::pair<int,int> goal;//�`�I��m
		char dire;//��V
		treeNode *superNode;//���`�I
		std::vector<treeNode*> subNode;//�l�`�I��C

	public:
		double weight;//�v��
		treeNode(std::pair<int,int> _position,std::pair<int,int> _goal,char _dire,treeNode *_superNode);
		std::vector<std::pair<int,int>> getCurrSnake(int length);//���o�����D��m
		treeNode *getSuper();//���o���`�I
		std::vector<treeNode*> getSub();//���o�l�`�I
		double distance(std::pair<int,int> start,std::pair<int,int> goal);//�p��Z��
		std::vector<char> getDireList();//��X��V��C
		void addNode(treeNode *N);//�s�W�l�`�I
		std::pair<int,int> getPosition();//���o��m
		~treeNode();
};