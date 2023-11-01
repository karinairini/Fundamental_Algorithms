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