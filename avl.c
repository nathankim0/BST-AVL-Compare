#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
typedef char BOOL;

struct avlNode {
	void* key;
	int height;   // 서브트리 높이

	struct avlNode* left;
	struct avlNode* right;
};

struct avlTree {
	unsigned int count;
	BOOL(*compare)(void* argu1, void* argu2);  // 왼쪽으로 보내야 되면 TRUE

	struct avlNode* root;
};

struct avlTree* CreateTree(BOOL(compFunc)(void*, void*));
void AddNode(struct avlTree* tree, void* data);
BOOL _insert(struct avlTree*, struct avlNode**, struct avlNode*);  
BOOL Compare(void*, void*);
void Traverse(struct avlNode*);
struct avlNode* leftBalance(struct avlNode*);
struct avlNode* rightBalance(struct avlNode*);
struct avlNode* rottLeft(struct avlNode*);
struct avlNode* rottRight(struct avlNode*);
int GetHeightDiff(struct avlNode*);
int getHeight(struct avlNode* node);

struct avlTree* CreateTree(BOOL(compFunc)(void*, void*))
{
	struct avlTree* newTree;

	newTree = (struct avlTree*)malloc(sizeof(struct avlTree));
	newTree->count = 0;
	newTree->root = 0;
	newTree->compare = compFunc;

	return newTree;
}
void AddNode(struct avlTree* tree, void* data)
{
	struct avlNode* node;

	node = (struct avlNode*)malloc(sizeof(struct avlNode));
	node->height = 1;
	node->key = data;
	node->left = 0;
	node->right = 0;

	if (tree->count == 0) {
		tree->root = node;
		tree->count++;
	}
	else
		_insert(tree, &(tree->root), node);
}
BOOL _insert(struct avlTree* tree, struct avlNode** root, struct avlNode* node)
{
	if (tree->compare((*root)->key, node->key)) {
		if ((*root)->left == 0) {
			(*root)->left = node;
			tree->count++;
			if ((*root)->right == 0) {
				(*root)->height++;

				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else {
			if (_insert(tree, &((*root)->left), node)) {
				(*root)->height++;
				if (GetHeightDiff((*root)) > 0)     // 왼쪽이 더 높으면
					(*root) = leftBalance((*root));    // 밸런싱
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}
	else {
		if ((*root)->right == 0) {
			(*root)->right = node;
			tree->count++;
			if ((*root)->left == 0) {
				(*root)->height++;

				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else {
			if (_insert(tree, &((*root)->right), node)) {
				(*root)->height++;
				if (GetHeightDiff((*root)) < 0)
					(*root) = rightBalance((*root));
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}
}
BOOL Compare(void* argu1, void* argu2)
{
	if ((*(int*)(argu1)) > (*(int*)(argu2)))
		return TRUE;
	else
		return FALSE;
}
void Traverse(struct avlNode* root)
{
	if (root == 0)
		return;

	printf("%d(%d)\n", *(int*)(root->key), GetHeightDiff(root));
	Traverse(root->left);
	Traverse(root->right);
}
struct avlNode* leftBalance(struct avlNode* root)
{

	if (GetHeightDiff(root->left) > 0)
		root = rottRight(root);
	else {
		root->left = rottLeft(root->left);
		root = rottRight(root);
	}

	return root;
}
struct avlNode* rightBalance(struct avlNode* root)
{

	if (GetHeightDiff(root->right) < 0)
		root = rottLeft(root);
	else {
		root->right = rottRight(root->right);
		root = rottLeft(root);
	}

	return root;
}
struct avlNode* rottRight(struct avlNode* root)
{
	struct avlNode* tmp;

	tmp = root->left;
	root->left = tmp->right;
	tmp->right = root;

	if (getHeight(root->left) > getHeight(root->right)) {
		root->height = getHeight(root->left) + 1;
	}
	else {
		root->height = getHeight(root->right) + 1;
	}

	if (getHeight(tmp->left) > getHeight(tmp->right)) {
		tmp->height = getHeight(tmp->left) + 1;
	}
	else {
		tmp->height = getHeight(tmp->right) + 1;
	}

	return tmp;
}
struct avlNode* rottLeft(struct avlNode* root)
{
	struct avlNode* tmp;

	tmp = root->right;
	root->right = tmp->left;
	tmp->left = root;

	if (getHeight(root->left) > getHeight(root->right)) {
		root->height = getHeight(root->left) + 1;
	}
	else {
		root->height = getHeight(root->right) + 1;
	}

	if (getHeight(tmp->left) > getHeight(tmp->right)) {
		tmp->height = getHeight(tmp->left) + 1;
	}
	else {
		tmp->height = getHeight(tmp->right) + 1;
	}

	return tmp;
}
int GetHeightDiff(struct avlNode* root)
{
	int left = 0;
	int right = 0;

	if (root->left != 0)
		left = root->left->height;

	if (root->right != 0)
		right = root->right->height;

	return left - right;
}
int getHeight(struct avlNode* node)
{
	if (node == 0)
		return 0;
	else
		return node->height;
}


int get_avl_Tree_height(struct avlNode* root) { // avl 트리
	if (!root)
		return 0;
	else {
		int left_h = get_avl_Tree_height(root->left); // 왼쪽 서브트리의 높이를 순환호출을 통해 얻는다.
		int right_h = get_avl_Tree_height(root->right); // 같은 방법으로 오른쪽 서브트리의 높이를 얻는다.
		return 1 + (left_h > right_h ? left_h : right_h); // 둘 중 큰 값에 1을 더해 반환한다.
	}
}

int* rand_num(num) {
	int i, temp, x, y;
	int* data = (int*)malloc(sizeof(int) * num);

	for (i = 0; i < num; i++) {
		data[i] = i + 1;
	}
	for (i = 0; i < 100; i++) {
		x = rand() % num;
		y = rand() % num;

		if (x != y) {
			temp = data[x];
			data[x] = data[y];
			data[y] = temp;
		}
	}
	return data;
}
int main()
{
	struct avlTree* avl_tree;
	avl_tree = CreateTree(Compare);
	srand((unsigned int)time(NULL));
	int num = 100;
	int* key;
	int* arr;
	arr = rand_num(num);
	for (int j = 0; j < num; j++) {
		//AVL입력
		key = (int*)malloc(sizeof(int));
		*key = arr[j];
		AddNode(avl_tree, (void*)key);
		//	Traverse(tree->root);
	}

	//printf("AVL 높이1 %d\n", height(avl_tree->root));
	printf("AVL 높이2 %d\n", get_avl_Tree_height(avl_tree->root));

	printf("\n");

	return 0;
}