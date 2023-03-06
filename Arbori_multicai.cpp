/*
You are required to implement correctly and efficiently linear time transformations between three different representations for a 
multi-way tree:
R1: parent representation: for each key you are given the parent key, in a vector.
R2: multi-way tree representation: for each node you have the key and a vector of children nodes
R3: binary tree representation: for each node, you have the key, and two pointers: one to the first child node, and one to the brother 
on the right (i.e. the next brother node)
Also, you are required to write a pretty print procedure on R3, which performs a preorder
traversal on the binary representation and outputs the tree in a friendly manner.
Therefore, you are given as input a multi-way tree in the parent representation (R1). You are required to implement T1, which transforms 
the tree to a multi-way representation (R2), then T2, which transforms from the multi-way representation to the binary representation 
(R3). Then, on the binary representation, you are asked to write a pretty print procedure (using a pre-order traversal).
You should be able to design the necessary data structures by yourselves. You may use intermediate structures (i.e. additional memory).
*/

#include <stdlib.h>
#include <stdio.h>

typedef struct Node_R2
{
	int key;
	int no_children; 
	struct Node_R2** children;
}Node_R2;

typedef struct Node_R3
{
	int key;
	struct Node_R3* left; //pointer catre primul copil
	struct Node_R3* right; //pointer catre fratele din dreapta
}Node_R3;

void pretty_print_R1(int* parent_array, int n, int parent, int level)
{
	for (int i = 0;i < n;i++)
	{
		if (parent_array[i] == parent)
		{
			for (int j = 0;j < level;j++)
				printf("\t");
			printf("%d\n", i + 1);
			pretty_print_R1(parent_array, n, i + 1, level + 1);
		}
	}
}

Node_R2* tranformare_T1(int* parent_array, int n, Node_R2** new_tree)
{
	Node_R2* root = (Node_R2*)malloc(sizeof(Node_R2));
	if (root != NULL)
	{
		for (int i = 0;i < n;i++)
		{
			new_tree[i] = (Node_R2*)malloc(sizeof(Node_R2));
			new_tree[i]->key = i + 1;
			new_tree[i]->no_children = 0;
			new_tree[i]->children = (Node_R2**)malloc(new_tree[i]->no_children*sizeof(Node_R2*));
		}
		for (int i = 0;i < n;i++)
		{
			if (parent_array[i] != -1)
				new_tree[parent_array[i] - 1]->no_children++;
			else
				root = new_tree[i];
		}
		for (int i = 0;i < n;i++)
		{
			if (new_tree[i]->no_children != 0)
			{
				new_tree[i]->children = (Node_R2**)malloc(new_tree[i]->no_children * sizeof(Node_R2*));
				new_tree[i]->no_children = 0;
			}
		}
		for (int i = 0;i < n;i++)
		{
			if (parent_array[i] != -1)
			{
				Node_R2* parent = (Node_R2*)malloc(sizeof(Node_R2));
				parent = new_tree[parent_array[i] - 1];
				parent->children[parent->no_children] = new_tree[i];
				parent->no_children++;
			}
		}
		return root;
	}
}

void pretty_print_R2(Node_R2* root, int level)
{
	if (root == NULL)
		return;
	for (int j = 0;j < level;j++)
		printf("\t");
	printf("%d\n", root->key);
	for (int i = 0;i < root->no_children;i++)
		pretty_print_R2(root->children[i], level + 1);
}

Node_R3* transformare_T2(Node_R2* root, Node_R3* leftBrother)
{
	Node_R3* root_T2 = (Node_R3*)malloc(sizeof(Node_R3));
	if (root_T2 != NULL)
	{
		root_T2->key = root->key;
		root_T2->left = NULL;
		root_T2->right = NULL;
		if (leftBrother != NULL)
			leftBrother->right = root_T2;
		Node_R3* child = NULL;
		for (int i = 0;i < root->no_children;i++)
		{
			child = transformare_T2(root->children[i], child);
			if (i == 0)
				root_T2->left = child;
		}
	}
	return root_T2;
}

void pretty_print_R3(Node_R3* root, int level)
{
	if (root == NULL)
		return;
	for (int j = 0;j < level;j++)
		printf("\t");
	printf("%d\n", root->key);
	pretty_print_R3(root->left, level + 1);
	pretty_print_R3(root->right, level);
}

int main()
{
	//three-way tree
	int parent_array[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2};
	pretty_print_R1(parent_array, 9, -1, 0);
	printf("\n");
	Node_R2** new_tree = (Node_R2**)malloc(9 * sizeof(Node_R2*));
	if (new_tree != NULL)
	{
		Node_R2* root_T1 = (Node_R2*)malloc(sizeof(Node_R2));
		if (root_T1 != NULL)
		{
			root_T1 = tranformare_T1(parent_array, 9, new_tree);
			for (int j = 0;j < 9;j++)
			{
				printf("%d: ", new_tree[j]->key);
				for (int i = 0;i < new_tree[j]->no_children;i++)
					printf("%d ", new_tree[j]->children[i]->key);
				printf("\n");
			}
			printf("\n");
			pretty_print_R2(root_T1, 0);
			printf("\n");
			Node_R3* root_T2 = (Node_R3*)malloc(sizeof(Node_R3));
			root_T2 = transformare_T2(root_T1, NULL);
			//printf("%d ", root_T2->key);
			//printf("%d ", root_T2->left->key);
			//printf("%d\n", root_T2->left->right->key);
			//printf("%d\n", root_T2->left->left->key);
			pretty_print_R3(root_T2, 0);
		}
	}
	return 0;
}