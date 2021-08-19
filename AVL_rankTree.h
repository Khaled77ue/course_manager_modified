//#include <iostream>
//using namespace std;
#define COUNT 10

typedef enum{
	AVL_SUCCESS, AVL_ALLOCATION_ERROR, AVL_INVALID_INPUT, AVL_FAILURE, AVL_NOT_EXIST
}AVL_RES;

template<class KeyType, class ItemType>
class AVL_TREE {
public:
	template<class KeyType1, class ItemType1>
	class AVLNode {
	public:
		AVLNode(KeyType key, ItemType* item) :
			balance_factor(0), Depth(0),w(0),m_Key(key) ,
			m_Data(item), x_tre_count(0), father_node(0),
			left_son(0), right_son(0), remb(0) {}

		~AVLNode() = default;

		int balance_factor;
		int Depth;
		int w;
		KeyType m_Key;
		ItemType* m_Data;
		int x_tre_count;
		AVLNode* father_node;
		AVLNode* left_son;
		AVLNode* right_son;
		int remb;
	};

	//methods :
	AVL_TREE() : Root(0),num_of_nodes(0) { }
	~AVL_TREE() {
		DestroyRecursive(Root);
		//delete m_pRoot;
		Root = nullptr;
	}
	void 	putNodeInMyTree(int x_tree, KeyType key, ItemType* item, int x);
	void 	RemoveInTree(KeyType key, int x);
	ItemType* FindInTree(KeyType key, int x);
	ItemType* select(int k);
	//void printTree(AVLNode< KeyType, ItemType >* root);
	AVLNode<KeyType, ItemType>* Root;
    void    cleara(AVLNode<KeyType, ItemType>* node);
    int num_of_nodes;
private:

	int wighUpdate(AVLNode<KeyType, ItemType>* root);
	int depthUpdate(AVLNode<KeyType, ItemType>* root);
	void BF_Update(AVLNode<KeyType, ItemType>* root);
	void updateAll(AVLNode<KeyType, ItemType>* root);
	void updateData_aux(AVLNode<KeyType, ItemType>* root);
	//void print_aux(AVLNode<KeyType, ItemType>* root, int space);

	AVLNode<KeyType, ItemType>* leafRemoveInTree(AVLNode<KeyType, ItemType>* node, int x);
	AVLNode<KeyType, ItemType>* sonRemoveInTreeSingle(AVLNode<KeyType, ItemType>* node, int x);
	void flipNodesInTree(AVLNode<KeyType, ItemType>* v, AVLNode<KeyType, ItemType>* w, int x);
	void DestroyRecursive(AVLNode<KeyType, ItemType>* node);
	AVL_RES DeleteInTree(AVLNode<KeyType, ItemType>* node, int x);
	ItemType* _Find_aux(KeyType key, AVLNode<KeyType, ItemType>* root, int x);
	void ComputeBF(AVLNode<KeyType, ItemType>* root, int x);
	void Balance(AVLNode<KeyType, ItemType>*& root, int* x);
	void BalanceRightSubTree(AVLNode<KeyType, ItemType>*& root, int x);
	void BalanceLeftSubTree(AVLNode<KeyType, ItemType>*& root, int x);
	void LeftSonRotation(AVLNode<KeyType, ItemType>*& root, int x);
	void RightSonRotation(AVLNode<KeyType, ItemType>*& root, int x);
	void aux_putNode(int x_tree, KeyType key, ItemType* item, AVLNode<KeyType, ItemType>*& root, int* x);
	int	GetDepth() const {return Root ? Root->Depth : 0;}
};
/*--------------------------------------------------------------------------------------------
									   destruction
  --------------------------------------------------------------------------------------------*/

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::cleara(AVLNode<KeyType, ItemType>* node) {
	DestroyRecursive(node);
	return;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::DestroyRecursive(AVLNode<KeyType, ItemType>* node) {
	if (node != nullptr)
	{
		if (node->left_son) {
			DestroyRecursive(node->left_son);
		}
		node->left_son = nullptr;

		if (node->right_son) {
			DestroyRecursive(node->right_son);
		}
		node->right_son = nullptr;
		if (node->m_Data) { delete node->m_Data; }

		node->m_Data = nullptr;

		if (node == Root) {
			if (node->m_Data) { delete node->m_Data; }
			delete node;

			Root = nullptr;

			return;
		}

		delete node;
	}
	return;
}

/*--------------------------------------------------------------------------------------------
									   insertion
  --------------------------------------------------------------------------------------------*/

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::putNodeInMyTree(int x_tree, KeyType key, ItemType* item, int x)
{
	if (Root == 0)
	{
		Root = new AVLNode<KeyType, ItemType>(key, item);
		Root->x_tre_count = x_tree;
		num_of_nodes=1;
        updateAll(this->Root);
	}
	else {
		int x = 0;
		aux_putNode(x_tree, key, item, Root, &x);
	}
	//updateAll(this->Root);
	//Balance(this->Root,&x);
	//updateAll(this->Root);
	Root->remb = x + 8;
	return;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::aux_putNode(int x_tree, KeyType key, ItemType* item, AVLNode<KeyType, ItemType>*& root, int* x)
{
	if (key < root->m_Key)
	{
		if (root->left_son) {
			aux_putNode(0, key, item, root->left_son,x);
		}
		else {
			root->left_son = new AVLNode<KeyType, ItemType>(key, item);

			num_of_nodes++;
			root->left_son->father_node = root;
			root->left_son->x_tre_count = x_tree;
			*x = root->right_son == nullptr ? 1 : 0;
			Balance(this->Root,x);
            updateAll(this->Root);
		}
		//root->w  += 1;
		//root->Depth += *x;
	}
	else if (key > root->m_Key)
	{
		if (root->right_son) {
			aux_putNode(0, key, item, root->right_son, x);
		}
		else {
			root->right_son = new AVLNode<KeyType, ItemType>(key, item);

			num_of_nodes++;
			root->right_son->father_node = root;
			root->right_son->x_tre_count = x_tree;
			*x = root->left_son == nullptr ? 1 : 0;
			Balance(this->Root,x);
            updateAll(this->Root);
		}
		//root->w += 1;
		//root->Depth += *x;
	}
	else {
		// error - can't have duplicate keys.
		// if duplicate keys are okay, change key < to key <= above
	}
	if (Root) {
		Root->remb = *x + 8;
	}

}

/*--------------------------------------------------------------------------------------------
										 removal
  --------------------------------------------------------------------------------------------*/

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::RemoveInTree(KeyType key, int x)
{
	AVLNode<KeyType, ItemType>* ptr = this->Root;
    if (ptr!= nullptr&&key == ptr->m_Key&&ptr->left_son== nullptr&&ptr->right_son== nullptr){
        delete ptr->m_Data;
        delete ptr;
        Root = nullptr;
        num_of_nodes--;
        return;
    }

	while (ptr != nullptr)
	{
		if (key == ptr->m_Key)
		{
			DeleteInTree(ptr, 0);
        //    Balance(this->Root,&x);
            updateAll(this->Root);
			return;
		}
		else if (key > ptr->m_Key)
		{
			ptr = ptr->right_son;
		}
		else
		{
			ptr = ptr->left_son;
		}
	}
	//updateAll(this->Root);
	Balance(this->Root,&x);
	updateAll(this->Root);
	if (Root) { Root->remb = x + 8; }
	return;
}

template<class KeyType, class ItemType>
AVL_RES AVL_TREE<KeyType, ItemType>::DeleteInTree(AVL_TREE<KeyType, ItemType>::AVLNode<KeyType, ItemType>* node, int x) {
    if(!FindInTree(node->m_Key,0))
        return AVL_NOT_EXIST;
	AVLNode<KeyType, ItemType>* start_point = nullptr;
	if (node->right_son == nullptr && node->left_son == nullptr) {
		start_point = leafRemoveInTree(node, 0); // go to father and delete node and w-=1 of father
	}
	else if (node->right_son != nullptr && node->left_son != nullptr) {
		AVLNode<KeyType, ItemType>* ptr = node->right_son;
		while (ptr->left_son != nullptr) {
			ptr = ptr->left_son;
		}
		flipNodesInTree(node, ptr, 0);
		if (node->right_son == nullptr && node->left_son == nullptr) {
			start_point = leafRemoveInTree(node, 0);
		}
		else {
			start_point = sonRemoveInTreeSingle(node, 0);
		}
	}
	else {
		start_point = sonRemoveInTreeSingle(node, 0);
	}

	if (Root == nullptr) {
		return AVL_SUCCESS;
	}

	if (start_point != Root){
		return AVL_SUCCESS;
	}
	else {

	}
	if (Root) {
		Root->remb = x + 7;
	}
	return AVL_SUCCESS;
}

template<class KeyType, class ItemType>
AVL_TREE<KeyType, ItemType>::AVLNode<KeyType, ItemType>* AVL_TREE<KeyType, ItemType>::leafRemoveInTree(AVL_TREE::AVLNode<KeyType, ItemType>* node, int x) {
	AVLNode<KeyType, ItemType>* start_point = nullptr;
	if (node == Root)
	{
		delete node->m_Data;
		delete node;
		Root = nullptr;
		num_of_nodes--;
		start_point = nullptr;
		node->w = 0;
		return start_point;
	}

	AVLNode<KeyType, ItemType>* father = node->father_node;
	start_point = father;
	if (father->right_son == node) {
		father->right_son = nullptr;
		father->w -= 1;
		if (node->m_Data) { delete node->m_Data; }
        num_of_nodes--;

		delete node;

		return start_point;
	}
	else {
		father->left_son = nullptr;
		father->w -= 1;
		if (node->m_Data) { delete node->m_Data; }
        num_of_nodes--;
		delete node;

		start_point = father;
		return start_point;
	}
	if (Root) {

		Root->remb = x + 8;
	}

	return start_point;
}

template<class KeyType, class ItemType>
AVL_TREE<KeyType, ItemType>::AVLNode<KeyType, ItemType>* AVL_TREE<KeyType, ItemType>::sonRemoveInTreeSingle(
	AVL_TREE<KeyType, ItemType>::AVLNode<KeyType, ItemType>* node, int x) {
	AVLNode<KeyType, ItemType>* start_point = nullptr;
	if (node->right_son != nullptr && node->left_son == nullptr)
	{
		if (node == Root)
		{int oldW=Root->w-1;
			Root = node->right_son;
			Root->w=oldW;
			delete node->m_Data;
            num_of_nodes--;
			delete node;
            node= nullptr;
			Root->father_node = nullptr;
			start_point = Root;
			return start_point;
		}
		else
		{
			// if node had one right_son son
			start_point = node->father_node;

			if (node->father_node->left_son == node)
			{
				node->father_node->left_son = node->right_son;
				node->right_son->father_node = node->father_node;
				//node->father_node->w = node->father_node->left_son->w + node->father_node->right_son->w;
				delete node->m_Data;
                num_of_nodes--;
				delete node;
                node= nullptr;
				return start_point;
			}
			else {
				node->father_node->right_son = node->right_son;
				node->right_son->father_node = node->father_node;
				//node->father_node->w = node->father_node->left_son->w + node->father_node->right_son->w;
				delete node->m_Data;
                num_of_nodes--;
				delete node;
                node= nullptr;
				return  start_point;
			}
		}
	}
	if (node->right_son == nullptr && node->left_son != nullptr)
	{
		if (node == Root) // OK
		{int sons=Root->w;
			Root = node->left_son;
			Root->w=sons-1;
			delete node->m_Data;
            num_of_nodes--;
			delete node;
            node= nullptr;
			Root->father_node = nullptr;
			start_point = Root;
			return start_point;
		}
		else {
			// if node had one right_son son
			start_point = node->father_node;
			if (node->father_node->m_Key > node->m_Key) {
				node->father_node->left_son = node->left_son;
				node->left_son->father_node = node->father_node;
				//node->father_node->w = node->father_node->left_son->w + node->father_node->right_son->w;
				delete node->m_Data;
                num_of_nodes--;
				delete node;
                node= nullptr;
				return start_point;
			}
			else {
				node->father_node->right_son = node->left_son;
				node->left_son->father_node = node->father_node;
				//node->father_node->w = node->father_node->left_son->w + node->father_node->right_son->w;
				delete node->m_Data;
                num_of_nodes--;
				delete node;
				node= nullptr;
				return start_point;
			}
		}
	}
	if (Root) {
		Root->remb = x + 8;
	}

	return start_point;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::flipNodesInTree(AVLNode<KeyType, ItemType>* v, AVLNode<KeyType, ItemType>* w, int x) {
	AVLNode<KeyType, ItemType>* father_nodeV = v->father_node;
	AVLNode<KeyType, ItemType>* father_nodeW = w->father_node;
	AVLNode<KeyType, ItemType>* leftV = v->left_son;
	AVLNode<KeyType, ItemType>* right_sonW = w->right_son;
	AVLNode<KeyType, ItemType>* leftW = w->left_son;

	if (v->right_son == w) {

		w->right_son = v;
		w->left_son = leftV;
		w->father_node = father_nodeV;
		v->father_node = w;
		v->left_son = leftW;
		if (father_nodeV != nullptr) {
			if (father_nodeV->m_Key > w->m_Key) {
				father_nodeV->left_son = w;
			}
			else {
				father_nodeV->right_son = w;
			}
		}
		leftV->father_node = w;

		if (w->right_son == nullptr) {
			v->right_son = nullptr;
			v->left_son = nullptr;
		}
		else {
			v->right_son = right_sonW;
			if (right_sonW != nullptr) {
				right_sonW->father_node = v;
			}
		}
	}

	//v.right_son != w
	else {
		leftV->father_node = w;
		w->left_son = leftV;
		w->father_node = father_nodeV;
		if (father_nodeV != nullptr) {
			if (father_nodeV->m_Key > w->m_Key) {
				father_nodeV->left_son = w;
			}
			else {
				father_nodeV->right_son = w;
			}
		}
		w->right_son = v->right_son;
		v->right_son->father_node = w;
		v->left_son = nullptr;
		v->right_son = right_sonW;
		v->father_node = father_nodeW;
		if (right_sonW != nullptr)
		{
			right_sonW->father_node = v;
		}
		if (father_nodeW->m_Key > v->m_Key) {
			father_nodeW->left_son = v;
		}
		else {
			father_nodeW->right_son = v;
		}

	}
	if (v == Root) {
		Root = w;
	}
	if (Root) {
		Root->remb = x + 8;
	}

}

/*--------------------------------------------------------------------------------------------
									  searching
  --------------------------------------------------------------------------------------------*/

template<class KeyType, class ItemType>
ItemType* AVL_TREE<KeyType, ItemType>::FindInTree(KeyType key, int x){

	if (Root) { Root->remb = x; }
	return _Find_aux(key, Root, 0);
}

template<class KeyType, class ItemType>
ItemType* AVL_TREE<KeyType, ItemType>::_Find_aux(KeyType key, AVLNode<KeyType, ItemType>* root, int x)
{
	if (root)

	{
		if (root->m_Key == key)
		{
			ItemType* res(nullptr);
			res = root->m_Data;
			return res;
		}

		if (key < root->m_Key)
			return _Find_aux(key, root->left_son, 0);
		else
			return _Find_aux(key, root->right_son, 0);
	}
	else
	{
		return nullptr;
	}
	if (Root) { Root->remb = x + 4; }

}

/*--------------------------------------------------------------------------------------------
								      balancing
  --------------------------------------------------------------------------------------------*/

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::ComputeBF(AVLNode<KeyType, ItemType>* root, int x)
{
	if (root)
	{
		int leftDepth = root->left_son ? root->left_son->Depth : 0;
		int right_sonDepth = root->right_son ? root->right_son->Depth : 0;

		root->Depth = 1 + ((leftDepth > right_sonDepth) ? leftDepth : right_sonDepth);
		root->balance_factor = right_sonDepth - leftDepth;
	}
	Root->remb = x;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::Balance(AVLNode<KeyType, ItemType>*& root, int* x)
{
	if (!root) {
		return;
	}
	Balance(root->left_son,x);
	Balance(root->right_son, x);

	if (root->balance_factor > 1) {
		BalanceRightSubTree(root, 1);
	}
	if (root->balance_factor < -1) {
		BalanceLeftSubTree(root, 1);
	}
	Root->remb = *x;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::BalanceRightSubTree(AVLNode<KeyType, ItemType>*& root, int x)
{
	if (root->right_son)
	{
		if (root->right_son->balance_factor > 0)//RR rotation
		{
			LeftSonRotation(root, 1);
		}
		else if (root->right_son&&root->right_son->balance_factor < 0)//RL rotation
		{
		RightSonRotation(root->right_son, 1);
			LeftSonRotation(root, 1);
		}
	}
	Root->remb = x;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::BalanceLeftSubTree(AVLNode<KeyType, ItemType>*& root, int x)
{
	if (root->left_son)
	{
		if (root->left_son->balance_factor < 0)//LL rotation
		{
			RightSonRotation(root, 1);
		}
		else if (root->left_son->balance_factor > 0)//LR rotation
		{
			LeftSonRotation(root->left_son, 1);
			RightSonRotation(root, 1);
		}
	}
	Root->remb = x;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::LeftSonRotation(AVLNode<KeyType, ItemType>*& root, int x)//RR basic rotation
{
	AVLNode<KeyType, ItemType>* pTemp = root;
	if (root && root->right_son) {
		root = root->right_son;
		pTemp->right_son = root->left_son;

		root->left_son = pTemp;
root->left_son->father_node=root;
		ComputeBF(root->left_son, 1);
		ComputeBF(root->right_son, 1);
		ComputeBF(root, 1);
	}
	Root->remb = x;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::RightSonRotation(AVLNode<KeyType, ItemType>*& root, int x)//LL basic rotation
{
	AVLNode<KeyType, ItemType>* pTemp = root;
	root = root->left_son;
	if(!root)
        return;
	    pTemp->left_son = root->right_son;

	root->right_son = pTemp;
	root->right_son->father_node= root;

	ComputeBF(root->left_son, 1);
	ComputeBF(root->right_son, 1);
	ComputeBF(root, 1);
	Root->remb = x;
}
/*--------------------------------------------------------------------------------------------
								  new methodes
  --------------------------------------------------------------------------------------------*/
template<class KeyType, class ItemType>
ItemType*  AVL_TREE<KeyType, ItemType> ::select(int k) {
	int w = 0;
	AVLNode<KeyType, ItemType>* v = this->Root;
	while (1) {// select algorithem !
	    if (v== nullptr) return nullptr;

		w = v->left_son == nullptr ? 0 : v->left_son->w + 1;
		if (w == k - 1) {
			return v->m_Data;
		}
		else {
			if (w > k - 1) {
				v = v->left_son;
			}
			else {
				k -= w + 1;
				v = v->right_son;
			}
		}
	}
}

template<class KeyType, class ItemType>
int AVL_TREE<KeyType, ItemType>::wighUpdate(AVLNode<KeyType, ItemType>* root) {
	if (root == nullptr) {
		return 0;
	}
	int left=0;
	int right=0;
	if(root->left_son){
	    left=wighUpdate(root->left_son);
	}
	if(root->right_son){
       right= wighUpdate(root->right_son);
	}
	root->w = left+right;
	return root->w +1;
}

template<class KeyType, class ItemType>
int AVL_TREE<KeyType, ItemType>::depthUpdate(AVLNode<KeyType, ItemType>* root) {
	if (root == nullptr) {
		return 0;
	}
	int left_depth =  depthUpdate(root->left_son);
	int right_depth = depthUpdate(root->right_son);
	if (!root->left_son && !root->right_son){
		root->Depth = 1;
	}
	else {
		root->Depth = left_depth > right_depth ? left_depth + 1 : right_depth + 1;
	}
	return root->Depth;
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::BF_Update(AVLNode<KeyType, ItemType>* root) {
	if (root == nullptr) {
		return ;
	}
	BF_Update(root->left_son);
	BF_Update(root->right_son);
	ComputeBF(root, 0);
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::updateAll(AVLNode<KeyType, ItemType>* root) {
	wighUpdate(root);
	depthUpdate(root);
	BF_Update(root);
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::updateData_aux(AVLNode<KeyType, ItemType>* root) {
	//wigh update
	if (root == nullptr) {
		return;
	}
	int total_left = root->left_son == nullptr ? 0 : root->left_son->w + 1;
	int total_right = root->right_son == nullptr ? 0 : root->right_son->w + 1;
	root->w = total_left + total_right;
	//depth update
	int d_left = root->left_son == nullptr ? 0 : root->left_son->Depth;
	int d_right = root->right_son == nullptr ? 0 : root->right_son->Depth;
	root->Depth = d_left > d_right ? d_left : d_right;
	ComputeBF(root, 0);
}

/*--------------------------------------------------------------------------------------------
								 tree print function
  --------------------------------------------------------------------------------------------*/

/*
template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::print_aux(AVLNode<KeyType, ItemType>* root, int space)
{
	if (root == nullptr)
		return;

	space += COUNT;

	print_aux(root->right_son, space);

	cout << endl;
	for (int i = COUNT; i < space; i++)
		 cout << " ";
	cout << *root->m_Data << "\n";

	print_aux(root->left_son, space);
}

template<class KeyType, class ItemType>
void AVL_TREE<KeyType, ItemType>::printTree(AVLNode < KeyType, ItemType >* root)
{
	print_aux(root, 0);
}

*/
