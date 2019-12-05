#define FALSE 0
#define TRUE 1
typedef char BOOL;

struct avlNode {
	void* key;
	int height;   // ¼­ºêÆ®¸® ³ôÀÌ

	struct avlNode* left;
	struct avlNode* right;
};

struct avlTree {
	unsigned int count;
	BOOL(*compare)(void* argu1, void* argu2);  // ¿ÞÂÊÀ¸·Î º¸³»¾ß µÇ¸é TRUE

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
				if (GetHeightDiff((*root)) > 0)     // ¿ÞÂÊÀÌ ´õ ³ôÀ¸¸é
					(*root) = leftBalance((*root));    // ¹ë·±½Ì
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
