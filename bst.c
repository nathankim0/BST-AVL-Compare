typedef struct bstbstNodeStruct
{
	int value;
	struct bstbstNodeStruct* leftChild;
	struct bstbstNodeStruct* rightChild;
}bstNode;

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
		// 자식 노드가 둘 다 있을 경우
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
	else if (root->value > value)
		return BST_search(root->leftChild, value);
	else
		return BST_search(root->rightChild, value);
}
void BST_print(bstNode* root)
{
	if (root == 0)
		return;

	printf("%d ", root->value);
	BST_print(root->leftChild);
	BST_print(root->rightChild);
}