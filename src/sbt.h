#include <set>
#include <vector>
#include <ctime>
#include <iostream>

using namespace std;

#define MAX 1000000
#define sz(node) (node == 0 ? 0 : node->size) 
struct sbTreeNode {
	sbTreeNode* left;
	sbTreeNode* right;
	int size;

	int val;
	sbTreeNode(int _val) :left(0), right(0), size(1) { val = _val; }
};

class SBtree {
public:
	SBtree();
	~SBtree();
	
	bool insert(int val)
	{
		return insert_aux(root, val);
	}
	
	bool erase(int val)
	{	return erase_aux(root, val);	}
	
	bool find(int val)
	{	return find_aux(root, val);		}
	
	// 返回在第 k 位置上的结点
	int select(int k)
	{	return select_aux(root, k);		}
	
	// 返回val在以root为根的树中的排名
	int rank(int val)
	{	return rank_aux(root, val);		}
	
	int pred(int val)
	{	return pred_aux(root, val);		}

	int succ(int val)
	{	return succ_aux(root, val);		}
	
	
private:
	bool rightRotate(sbTreeNode*& cur);
	bool leftRotate(sbTreeNode*& cur);

	bool insert_aux(sbTreeNode*& cur, int val);
	bool erase_aux(sbTreeNode* cur, int val);
	bool find_aux(sbTreeNode* cur, int val);
	int  select_aux(sbTreeNode* cur, int k);
	int  rank_aux(sbTreeNode* cur, int val);
	bool pred_aux(sbTreeNode* cur, int val);
	bool succ_aux(sbTreeNode* cur, int val);
	void destory(sbTreeNode* cur);

private:

	sbTreeNode* root;
};


SBtree::SBtree() :root(0) {

}

SBtree ::~SBtree () {
	destory(root);
	root = NULL;
}

//		cur: x									  cur: y
//		  x											y
//		 / \     ------------>rightRotate		   / \
//		y   c									  a   x
//	   / \		 leftRotate<-------------			 / \
//    a   b											b   c

bool SBtree::rightRotate(sbTreeNode*& cur) {
	
	sbTreeNode* temp = cur->left;
	cur->left = temp->right;
	temp->right = cur;

	temp->size = cur->size;
	cur->size = sz(cur->left) + sz(cur->right) + 1;
	cur = temp;

	return true;
}

bool SBtree::leftRotate(sbTreeNode*& cur) {
	sbTreeNode* temp = cur->right;
	cur->right = temp->left;
	temp->left = cur;

	temp->size = cur->size;
	cur->size = sz(cur->left) + sz(cur->right) + 1;
	cur = temp;

	return true;
}

bool SBtree::insert_aux(sbTreeNode*& cur, int val){
	
	if (cur == NULL) {
		cur = new sbTreeNode(val);
	} else {
		cur->size++;
		if (cur->val > val){

			insert_aux(cur->left, val);
			
			if (cur->left != NULL &&
			   ( (sz(cur->left->left) > sz(cur->right))  || 
			     (sz(cur->left->right) > sz(cur->right)) )) {
				//cout << "\nrr    ----: " << cur->val << "\n";
				rightRotate(cur);
			}
		} else {

			insert_aux(cur->right, val);
			
			if (cur->right != NULL && 
			   ( (sz(cur->right->right) > sz(cur->left)) ||
				 (sz(cur->right->left) > sz(cur->left))  )){
				//cout << "\nlr    ----: " << cur->val << "\n";
				leftRotate(cur);
			}
		}
	}

	return true;
}

bool SBtree::erase_aux(sbTreeNode* cur, int val){
	return true;
}

bool SBtree::find_aux(sbTreeNode* cur, int val) {
	while (cur != NULL) {
		if (cur->val == val)
			return true;
		else if (cur->val > val)
			cur = cur->left;
		else
			cur = cur->right;
	}

	return false;
}
int SBtree::select_aux(sbTreeNode* cur, int k){
	//Select K 大于SBT的个数
	if (cur->size < k)
		return -1;

	if (sz(cur->left) + 1 > k)
		return select_aux(cur->left, k);
	else if (sz(cur->left) + 1 < k)
		return select_aux(cur->right, k - sz(cur->left) -1);
	else
		return cur->val;

	return -1;
}

int SBtree::rank_aux(sbTreeNode* cur, int val){
	if (cur->val == val)
		return sz(cur->left) + 1;
	else if (cur->val > val)
		return rank_aux(cur->left, val);
	else
		return sz(cur->left) + 1 + rank_aux(cur->right, val);

	return -1;
}


void SBtree::destory(sbTreeNode* cur){
	if (cur != NULL) {
		destory(cur->left);
		destory(cur->right);
		delete cur;
	}

}

int main() {
	//	srand((unsigned)time(NULL));
	vector<int> vec;
	set<int> s;
	SBtree sb;

	for (int i = 0; i < 1000000; ++i)
		vec.push_back(rand() % 10000000);
	
	clock_t start, finish;
	clock_t start1, finish1;

	start = clock();
	for (int i = 0; i < MAX; ++i){
		sb.insert(vec[i]);
	}
		
	finish = clock();

	start1 = clock();
	for (int i = 0; i < MAX; ++i){
		s.insert(vec[i]);
	}
	finish1 = clock();

	cout << finish - start << "   " << finish1 - start1 << endl;
	cout << (double)(finish - start);
	cout << sb.select(2)<<" ";
	cout << sb.rank(24470);
	getchar();
	return 0;
}