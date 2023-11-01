#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler pro("StaticiDinamiceDeOrdine");

#define STEP_SIZE 100
#define MAX_SIZE 10000
#define NR_TESTS 5

typedef struct _Node
{
	int key;
	struct _Node* left;
	struct _Node* right;
	int size; //dimensiunea sub-arborelui ce are nodul ca rădăcină
}Node;

Node* createNode(int key)
{
	Node* node = (Node*)malloc(sizeof(Node));
	if (node != NULL)
	{
		node->key = key;
		node->left = NULL;
		node->right = NULL;
		node->size = 0;
		return node;
	}
}

Node* BUILD_TREE(int a, int b, int* array)
{
	//plaseaza elementul din mijloc ca si radacina, in cazul nostru pe 6
	if (a > b)
	{
		return NULL;
	}
	int k = (a + b) / 2;
	/*
	construieste subarborele din stanga cu elemenetele mai mici decat k, iar dupa subarborele din dreapata cu 
	elementele mai mari decat k
	*/
	Node* root = createNode(array[k]);	
	root->left = BUILD_TREE(a, k - 1, array);
	root->right = BUILD_TREE(k + 1, b, array);

	//daca arborele este format doar din radacina, size = 1
	if (root->left == NULL && root->right == NULL)
		root->size = 1;

	if (root->left != NULL && root->right != NULL)
		root->size = root->right->size + root->left->size + 1;
	else
		if (root->left != NULL)
			root->size = root->left->size + 1;
		else
			if (root->right != NULL)
				root->size = root->right->size + 1;
	return root;
}

void pretty_print(Node* root, int level)
{
	if (root == NULL)
		return;
	if(root->right!=NULL)
		pretty_print(root->right, level + 1);
	for (int i = 0;i < level;i++)
		printf("\t");
	printf("%d\n", root->key);
	pretty_print(root->left, level + 1);
}

void pretty_print_2(Node* root, int level)
{
	if (root == NULL)
		return;
	for (int i = 0;i < level;i++)
		printf("\t");
	printf("%d\n", root->key);
	pretty_print_2(root->right, level + 1);
	pretty_print_2(root->left, level + 1);
}

//selectează elementul cu a i-a cea mai mică cheie
Node* OS_SELECT(Node* root, int i, int n)
{
	Operation opComp = pro.createOperation("OS_SELECT-comp", n);
	Operation opAttr = pro.createOperation("OS_SELECT-attr", n);
	//daca se introduce o valoarea mai mica decat 1 sau mai mare decat n, cel mai mare element din arbore,
	//pentru rank nu e ok
	if (i < 1 || i > n)
		return NULL;
	opComp.count();
	//arbore vid
	if (root == NULL)
		return root;
	//daca radacina curenta nu are subarbore stang, inseamna ca rank-ul lui este 1
	int rank = 1;
	//rank ul unui nod reprezinta numarul de noduri mai mici decat el + el insusi
	opComp.count();
	if (root->left != NULL)
		rank = root->left->size + 1; //numarul de noduri pe stanga + 1 (nodul insine)
	if (i == rank) //inseamna ca e radacina curenta
		return root;
	else if (i < rank) //altfel al i-lea element cel mai mic se afla in subarborele stanga
		return OS_SELECT(root->left, i, n);
	//altfel al i-lea element cel mai mic se afla in subarborele dreapta 
	//al i-lea elemenent cel mai mic este al (i-rank)-lea cel mai mic element din subarborele din dreapta
	else if (i > rank) 
		return OS_SELECT(root->right, i - rank, n);
}

//succesorul este cel mai stanga din subarborele din dreapta
Node* findSuccessor(Node* root, Operation* opComp, Operation* opAttr)
{
	opAttr->count();
	Node* succ = root;
	while (succ->left != NULL) 
	{
		opComp->count();
		opAttr->count();
		succ = succ->left;
	}
	opComp->count();
	return succ;
}

//sterge elementul cu cheia i
Node* OS_DELETE(Node* root, int i, int n)
{
	Operation opComp = pro.createOperation("OS_DELETE-comp", n);
	Operation opAttr = pro.createOperation("OS_DELETE-attr", n);
	//daca se introduce o valoarea mai mica decat 1 sau mai mare decat n, cel mai mare element din arbore,
	//pentru rank nu e ok
	if (i<1 || i>n)
		return NULL;
	opComp.count();
	//arbore vid
	if (root == NULL)
		return root;
	//daca cheia i este mai mica decat cheia radacinii, cautarea nodului i se face in subarborele stang
	opComp.count();
	if (i < root->key)
	{
		root->left = OS_DELETE(root->left, i, n);
		root->size--;
	}
	//daca cheia i este mai mare decat cheia radacinii, cautarea nodului i se face in subarborele drept
	else
	{
		opComp.count();
		if (i > root->key)
		{
			root->right = OS_DELETE(root->right, i, n);
			root->size--;
		}
		//daca cheia radacinii este egala cu i, atunci acesta este nodul ce trebuie sters
		else
		{
			opComp.count();
			if (i == root->key)
			{
				root->size--;
				//daca nodul este frunza - se elimina
				opComp.count(3);
				if (root->left == NULL && root->right == NULL)
				{
					free(root);
					return NULL;
				}
				//nodul are un singur fiu
				if (root->right == NULL)
				{
					opAttr.count();
					Node* aux = root->left;
					free(root);
					return aux;
				}
				if (root->left == NULL)
				{
					opAttr.count();
					Node* aux = root->right;
					free(root);
					return aux;
				}
				opAttr.count(3);
				//nodul are 2 fii, il inlocuim cu succesorul lui
				Node* succ = findSuccessor(root->right, &opComp, &opAttr);
				root->key = succ->key;
				//dupa ce inlocuieste radacina cu succesorul, continua cu stergerea succesorului
				root->right = OS_DELETE(root->right, succ->key, n);
			}
		}
	}
	return root;
}

void demo()
{
	int array[11] = { 1,2,3,4,5,6,7,8,9,10,11 };
	int n = 11;
	Node* root = BUILD_TREE(0, 10, array);
	pretty_print(root, 0);
	pretty_print_2(root, 0);
	printf("%d\n", root->size);

	int i = 9;
	Node* selected = OS_SELECT(root, i, n);
	if (selected != NULL)
		printf("Elementul cu a %d-a cea mai mica cheie este: %d.\n", i, selected->key);
	else 
		printf("Elementul cu a %d-a cea mai mica cheie nu s-a putut selecta.\n", i);

	i = 5;
	selected = OS_SELECT(root, i, n);
	if (selected != NULL)
		printf("Elementul cu a %d-a cea mai mica cheie este: %d.\n", i, selected->key);
	else
		printf("Elementul cu a %d-a cea mai mica cheie nu s-a putut selecta.\n", i);

	i = 6;
	selected = OS_SELECT(root, i, n);
	if (selected != NULL)
		printf("Elementul cu a %d-a cea mai mica cheie este: %d.\n", i, selected->key);
	else
		printf("Elementul cu a %d-a cea mai mica cheie nu s-a putut selecta.\n", i);
	
	i = 5;
	root = OS_DELETE(root, i, n);
	if (root != NULL)
		printf("Elementul cu cheia %d a fost sters.\n", i);
	else
		printf("Elementul cu cheia %d a nu a putut fi sters.\n", i);
	pretty_print(root, 0);
	printf("%d\n", root->size);

	i = 9;
	root = OS_DELETE(root, i, n);
	if (root != NULL)
		printf("Elementul cu cheia %d a fost sters.\n", i);
	else
		printf("Elementul cu cheia %d a nu a putut fi sters.\n", i);
	pretty_print(root, 0);
	printf("%d\n", root->size);

	i = 6;
	root = OS_DELETE(root, i, n);
	if (root != NULL)
		printf("Elementul cu cheia %d a fost sters.\n", i);
	else
		printf("Elementul cu cheia %d a nu a putut fi sters.\n", i);
	pretty_print(root, 0);
	printf("%d\n", root->size);

}

void op_management()
{
	for (int n = 100;n <= MAX_SIZE;n += STEP_SIZE)
	{
		for (int test = 0;test < NR_TESTS;test++)
		{
			int* array = (int*)malloc(n * sizeof(int));
			if (array != NULL)
			{
				for (int i = 1;i <= n;i++)
				{
					array[i] = i;
				}
				Node* root = BUILD_TREE(0, n - 1, array);
				int* array_aux = (int*)malloc(n * sizeof(int));
				if (array_aux != NULL)
				{
					FillRandomArray(array_aux, n, 1, n, true, 0);
					for (int j = 0;j < n;j++)
					{
						Node* selected = OS_SELECT(root, array_aux[j], n);
						root = OS_DELETE(root, array_aux[j], n);
					}
				}
			}
		}
	}
	pro.divideValues("OS_SELECT-comp", NR_TESTS);
	pro.divideValues("OS_SELECT-attr", NR_TESTS);

	pro.divideValues("OS_DELETE-comp", NR_TESTS);
	pro.divideValues("OS_DELETE-attr", NR_TESTS);

	pro.addSeries("TOTAL-OS_SELECT", "OS_SELECT-comp", "OS_SELECT-attr");
	pro.addSeries("TOTAL-OS_DELETE", "OS_DELETE-comp", "OS_DELETE-attr");

	pro.showReport();
}

int main()
{
	//demo();
	op_management();
	return 0;
}