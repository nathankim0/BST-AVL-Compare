#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define MAX(a,b) (((a)>(b))?(a):(b))
typedef char BOOL;

/*BST 구조체*/
typedef struct bstNodeStruct
{
	int value;
	struct bstNodeStruct* leftChild;
	struct bstNodeStruct* rightChild;
}bstNode;

/*AVL 구조체*/
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

/*탐색 횟수 체크 위한 전역변수*/
int bst_search_cnt = 0, avl_search_cnt=0;

/*사용될 함수들*/
int get_bst_Tree_height(bstNode* root);
int get_avl_Tree_height(struct avlNode* root);
int* rand_num(num);

void func_bst(bstNode* tree, int* arr, int num);
void func_avl(struct avlTree* tree, int* arr, int num);


bstNode* BST_insert(bstNode* root, int value);
bstNode* findMinbstNode(bstNode* root);
bstNode* BST_delete(bstNode* root, int value);
bstNode* BST_search(bstNode* root, int value);
void BST_print(bstNode* root);

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
struct avlNode* AVL_search(struct avlNode* root, int value);



/**************************main**************************/
/*                                                      */
/*                                                      */
/*                                                      */
/*                                                      */
/********************************************************/

int main()
{
	//100~10000 까지 500씩 증가
	int n = 0;
	for (int i = 100; i <= 5000; i = 500*n) {
		n++;
		printf("*****************노드개수:[%d]*****************\n", i);

		bstNode* bst_tree = NULL;

		struct avlTree* avl_tree;
		int* key;
		avl_tree = CreateTree(Compare);

		int* arr;
		//배열에 i만큼 랜덤값 입력
		arr = rand_num(i);

		//랜덤 데이터 입력
		for (int j = 0; j < i; j++) {
			//BST입력
			bst_tree = BST_insert(bst_tree, arr[j]);

			//AVL입력
			key = (int*)malloc(sizeof(int));
			*key = arr[j];
			AddNode(avl_tree, (void*)key);
			//Traverse(avl_tree->root);
		}
		//랜덤 숫자 탐색 횟수의 평균 (100회 수행)
		func_bst(bst_tree, arr, i);
		func_avl(avl_tree, arr, i);


		//BST_print(bst_tree);
		printf("\n");
		printf("bst 높이: %d\n", get_bst_Tree_height(bst_tree));
		printf("avl 높이: %d\n\n", get_avl_Tree_height(avl_tree->root));
	}
	return 0;
}



/********************************************************/
/*                                                      */
/*                                                      */
/*                                                      */
/*                                                      */
/********************************************************/

int get_bst_Tree_height(bstNode* root) { // bst 트리의 높이
	if (!root)
		return 0;
	else {
		int left_h = get_bst_Tree_height(root->leftChild); // 왼쪽 서브트리의 높이를 순환호출을 통해 얻는다.
		int right_h = get_bst_Tree_height(root->rightChild); // 같은 방법으로 오른쪽 서브트리의 높이를 얻는다.
		return 1 + (left_h > right_h ? left_h : right_h); // 둘 중 큰 값에 1을 더해 반환한다.
	}
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
	srand((unsigned int)time(NULL));

	for (i = 0; i < num; i++) {
		data[i] = i + 1;
	}
	for (i = 0; i < 20; i++) {
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

/*BST 랜덤 숫자 탐색 횟수의 평균 (100회 수행)*/
void func_bst(bstNode* tree, int* arr, int num) {
	int tmp;
	double sum_cnt = 0;
	for (int i = 0; i < 100; i++) {
		bst_search_cnt = 0;
		tmp = rand() % num;

		BST_search(tree, tmp);
		sum_cnt += bst_search_cnt;
		//printf("bst cnt: %d\n", bst_search_cnt);

	}
	printf("BST 랜덤 숫자 탐색 횟수의 평균 (100회 수행): %.1f\n", sum_cnt / 100);
}

/*AVL 랜덤 숫자 탐색 횟수의 평균 (100회 수행)*/
void func_avl(struct avlTree* tree, int* arr, int num) {
	int tmp;
	double sum_cnt = 0;
	for (int i = 0; i < 100; i++) {
		avl_search_cnt = 0;
		tmp = rand() % num;

		AVL_search(tree->root, tmp);
		sum_cnt += avl_search_cnt;
		//printf("avl cnt: %d\n", avl_search_cnt);
	}
	printf("AVL 랜덤 숫자 탐색 횟수의 평균 (100회 수행): %.1f\n", sum_cnt / 100);
}



/**************************BST**************************/
/*                                                     */
/*                                                     */
/*                                                     */
/*                                                     */
/*******************************************************/


bstNode* root = 0;

bstNode* BST_insert(bstNode* root, int value)
{
	if (root == 0)
	{
		root = (bstNode*)malloc(sizeof(bstNode));
		root->leftChild = root->rightChild = 0;
		root->value = value;
		return root;
	}
	else
	{
		if (root->value > value)
			root->leftChild = BST_insert(root->leftChild, value);
		else
			root->rightChild = BST_insert(root->rightChild, value);
	}
	return root;
}
bstNode* findMinbstNode(bstNode* root)
{
	bstNode* tmp = root;
	while (tmp->leftChild != 0)
		tmp = tmp->leftChild;
	return tmp;
}
bstNode* BST_delete(bstNode* root, int value)
{
	bstNode* tbstNode = 0;
	if (root == 0)
		return 0;

	if (root->value > value)
		root->leftChild = BST_delete(root->leftChild, value);
	else if (root->value < value)
		root->rightChild = BST_delete(root->rightChild, value);
	else
	{
		if (root->rightChild != 0 && root->leftChild != 0)
		{
			tbstNode = findMinbstNode(root->rightChild);
			root->value = tbstNode->value;
			root->rightChild = BST_delete(root->rightChild, tbstNode->value);
		}
		else
		{
			tbstNode = (root->leftChild == 0) ? root->rightChild : root->leftChild;
			free(root);
			return tbstNode;
		}
	}

	return root;
}
bstNode* BST_search(bstNode* root, int value)
{
	if (root == 0)
		return 0;

	if (root->value == value)
		return root;
	else if (root->value > value) {
		++bst_search_cnt;
		return BST_search(root->leftChild, value);
	}
	else {
		++bst_search_cnt;
		return BST_search(root->rightChild, value);
	}
}
void BST_print(bstNode* root)
{
	if (root == 0)
		return;

	printf("%d ", root->value);
	BST_print(root->leftChild);
	BST_print(root->rightChild);
}




/**************************AVL**************************/
/*                                                     */
/*                                                     */
/*                                                     */
/*                                                     */
/*******************************************************/

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
				if (GetHeightDiff((*root)) > 0)
					(*root) = leftBalance((*root));
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

	//printf("%d(%d)\n", *(int*)(root->key), GetHeightDiff(root));
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
struct avlNode* AVL_search(struct avlNode* root, int value)
{
	if (root == 0)
		return 0;

	if (*(int*)root->key == value)
		return root;
	else if (*(int*)root->key > value) {
		++avl_search_cnt;
		return AVL_search(root->left, value);
	}
	else {
		++avl_search_cnt;
		return AVL_search(root->right, value);
	}
}
