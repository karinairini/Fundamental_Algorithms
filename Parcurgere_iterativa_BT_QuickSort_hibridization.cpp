#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler pro("Parcurgere_iterativa_BT_QuickSort_hibridization");

#define MAXSIZE 10000
#define STEPSIZE 100
#define NR_TESTS_1 5
#define NR_TESTS_2 100

typedef struct Node_AB
{
	int key;
	struct Node_AB* left;
	struct Node_AB* right;
}Node_AB;

//am implementat stiva ca o lista simplu inlantuita
typedef struct Node_stack
{
	Node_AB* top; //pointer la primul nod din stiva
	struct Node_stack* next;
}Node_Stack;

Node_AB* create_node_AB(int key)
{
	Node_AB* node = (Node_AB*)malloc(sizeof(Node_AB));
	if (node != NULL)
	{
		node->key = key;
		node->left = NULL;
		node->right = NULL;
	}
	return node;
}

Node_AB* AB_insert_node(Node_AB* root, int key)
{
	Node_AB* node = create_node_AB(key);
	//daca arborele este vid, radacina reprezinta nodul
	if (root == NULL)
		return node;
	//daca arborele nu este vid, comparam cheia nodului pe care il inseram cu cheia radacinii
	//daca cheia nodului este mai mica decat cheia radacinii, il inseram in subarborele din stanga
	if (node->key < root->key)
		root->left = AB_insert_node(root->left, key);
	else
	//altfel, il inseram in subarborele din dreapta
		root->right = AB_insert_node(root->right, key);
	return root;
}

void pretty_print(Node_AB* root, int level)
{
	if (root == NULL)
		return;
	pretty_print(root->right, level + 1);
	for (int i = 0;i < level;i++)
		printf("\t");
	printf("%d\n", root->key);
	pretty_print(root->left, level + 1);
}

void preorder_recursiv(Node_AB* root, int n)
{
	//procesam nodul, traversam subarborele din stanga, dupa subarborele din dreapta
	Operation op = pro.createOperation("Preordine_recursiv", n);
	if (root != NULL)
	{
		op.count();
		printf("%d ", root->key);
		preorder_recursiv(root->left, n);
		preorder_recursiv(root->right, n);
	}
}

void inorder_recursiv(Node_AB* root, int n)
{
	//traversam subarborbele din stangal, procesam nodul, dupa traversam subarborele din dreapta
	Operation op = pro.createOperation("Inordine_recursiv", n);
	if (root != NULL)
	{
		inorder_recursiv(root->left, n);
		op.count();
		printf("%d ", root->key);
		inorder_recursiv(root->right, n);
	}
}

void postorder_recursiv(Node_AB* root, int n)
{
	//traversam subarborele din stanga, subarborele din dreapta, iar la final procesam nodul
	Operation op = pro.createOperation("Postordine_recursiv", n);
	if (root != NULL)
	{
		postorder_recursiv(root->left, n);
		postorder_recursiv(root->right, n);
		op.count();
		printf("%d ", root->key);
	}
}

int isStackEmpty(Node_stack* stack)
{
	if (stack == NULL)
		return 1;
	return 0;
}

//insert first
void push(Node_stack** stack, Node_AB* node_tree)
{
	//alocam nodul pe care il introducem in stiva
	Node_stack* node = (Node_stack*)malloc(sizeof(Node_stack));
	if (node != NULL)
	{
		//punem in noul nod din stiva datele nodului din arbore
		node->top = node_tree;
		node->next = (*stack);
		*stack = node;
	}
}

//delete first
void pop(Node_stack** stack)
{
	//daca stiva este goala
	if (isStackEmpty(*stack) == 1)
		return;
	Node_stack* aux = *stack;
	Node_AB* top_aux = aux->top;
	*stack = aux->next;
	free(aux);
}

void preorder_iterativ(Node_AB* root, int n)
{
	Operation op = pro.createOperation("Preordine_iterativ", n); 
	//daca arborele nu este vid
	if (root != NULL)
	{
		//initializam stiva
		Node_stack* stack = NULL;
		//adaugam radacina in stiva
		push(&stack, root);
		while (isStackEmpty(stack) != 1)
		{
			//afisam ultimul nod inserat in stiva, iar dupa il stergem din stiva
			Node_AB* node = stack->top;
			op.count();
			printf("%d ", node->key);
			pop(&stack);

			//pune prima data pe stiva copilul din dreapta si dupa copilul din stanga, pentru ca la urmatoarea iteratie
			//primul nod din stiva sa fie copilul din stanga
			
			//daca ultimul nod inserat in stiva are copil in partea dreapta, il adaugam in stiva
			if (node->right != NULL)
				push(&stack, node->right);
			//daca ultimul nod inserat in stiva are copil in partea stanga, il adaugam in stiva
			if (node->left != NULL)
				push(&stack, node->left);
		}
	}
}

void inorder_iterativ(Node_AB* root, int n)
{
	Operation op = pro.createOperation("Inordine_iterativ", n);
	//daca arborele nu este vid
	if (root != NULL)
	{
		//initializam stiva
		Node_stack* stack = NULL;
		while (isStackEmpty(stack) == 0 || root != NULL)
		{
			//mergem pana la ultimul copil de pe stanga al nodului curent
			while (root != NULL)
			{
				//adaugam copii de pe stanga in stiva
				push(&stack, root);
				root = root->left;
			}
			//afisam pe rand copii din stanga, eliminandu-i pe rand din stiva, primul afisat fiind de fapt ultimul copil din arbore 
			root = stack->top;
			op.count();
			printf("%d ", root->key);
			pop(&stack);
			//dupa ce am vizitat nodul si am parcurs subarborele din stanga, facem la fel pentru subarborele din dreapta
			root = root->right;
		}
	}
}

void postorder_iterativ(Node_AB* root, int n)
{
	Operation op = pro.createOperation("Preordine_iterativ", n);
	//daca arborele nu este vid
	if (root != NULL)
	{
		//initializam doua stive
		//stack1 pentru procesarea nodurilor si a copiilor
		Node_stack* stack1 = NULL;
		//stack2 pentru nodurile procesate
		Node_stack* stack2 = NULL;
		//adaugam radacina in prima stiva
		push(&stack1, root);
		//cat timp prima stiva nu este goala, eliminam pe rand cate un element din ea si il adaugam in a doua stiva
		while (isStackEmpty(stack1) != 1)
		{
			Node_AB* node = stack1->top;
			pop(&stack1);
			push(&stack2, node);

			//adaugam in prima stiva copiii din stanga si dreapta ai nodului eliminat, daca acestia exista, 
			if (node->left != NULL)
				push(&stack1, node->left);
			if (node->right != NULL)
				push(&stack1, node->right);
		}
		//cat timp a doua stiva nu este goala, afisam elementele din ea
		while (isStackEmpty(stack2) == 0)
		{
			op.count();
			printf("%d ", stack2->top->key);
			pop(&stack2);
		}
	}
}

void demoParcurgeriArbore()
{
	int array[9] = { 14,6,17,4,13,15,20,8,31 };
	int n = 9;
	Node_AB* root = NULL;
	for (int i = 0;i < n;i++)
		root = AB_insert_node(root, array[i]);
	pretty_print(root, 0);
	
	printf("\nParcurgerea (recursiva) in preordine a arborelui este: ");
	preorder_recursiv(root, n);
	
	printf("\n\nParcurgerea (recursiva) in inordine a arborelui este: ");
	inorder_recursiv(root, n);

	printf("\n\nParcurgerea (recursiva) in postordine a arborelui este: ");
	postorder_recursiv(root, n);

	printf("\n\nParcurgerea (iterativa) in preordine a arborelui este: ");
	preorder_iterativ(root, n);

	printf("\n\nParcurgerea (iterativa) in inordine a arborelui este: ");
	inorder_iterativ(root, n);

	printf("\n\nParcurgerea (iterativa) in postordine a arborelui este: ");
	postorder_iterativ(root, n);
}

void parcurgeriOperatii()
{
	for (int n = STEPSIZE;n <= MAXSIZE;n += STEPSIZE)
	{
		int* array = (int*)malloc(n * sizeof(int));
		if (array != NULL)
		{
			FillRandomArray(array, n, 1, n, true, 0);
			Node_AB* root = NULL;
			for (int i = 0;i < n;i++)
				root = AB_insert_node(root, array[i]);
			preorder_recursiv(root, n);
			preorder_iterativ(root, n);
			inorder_recursiv(root, n);
			inorder_iterativ(root, n);
			postorder_recursiv(root, n);
			postorder_iterativ(root, n);
		}
	}
	pro.showReport();
}

void print_array(int* array, int n)
{
	for (int i = 0;i < n;i++)
		printf("%d ", array[i]);
	printf("\n");
}

void insertionSort(int* array, int p, int n, Operation* opComp, Operation* opAttr)
{
	for (int i = p + 1;i < n + 1;i++)
	{
		opAttr->count();
		int key = array[i];
		int j = i - 1;
		while (j >= p && array[j] > key)
		{
			opComp->count();
			opAttr->count();
			array[j + 1] = array[j];
			j = j - 1;
		}
		opComp->count();
		opAttr->count();
		array[j + 1] = key;
	}
}

/*
Functia PARTITION ia ca și pivot ultimul element, îl plasează în poziția corectă în șirul sortat și mută 
elementele mai mici decât pivotul, la stânga lui, iar elementele mai mari la dreapta lui.
*/
int PARTITION(int* array, int p, int r, int n, Operation* opComp, Operation* opAttr)
{
	//Lomuto partition
	opAttr->count();
	//x - pivotul este ultimul element al sirului
	int x = array[r];
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		opComp->count();
		//daca elementul curent este mai mic sau egal cu pivotul
		if (array[j] <= x)
		{
			i++; //crește indexul elementului mai mic
			opAttr->count(3);
			int aux_1 = array[i];
			array[i] = array[j];
			array[j] = aux_1;
		}
	}
	opAttr->count(3);
	int aux_2 = array[i + 1];
	array[i + 1] = array[r];
	array[r] = aux_2;
	return i + 1;
}

void quickSort(int* array, int p, int r, int n)
{
	Operation opComp = pro.createOperation("QuickSort-comp", n);
	Operation opAttr = pro.createOperation("QuickSort-attr", n);
	//sorteaza prima parte pana unde s-a inserat pivotul si dupa partea de dupa pivot
	if (p < r)
	{
		int q = PARTITION(array, p, r, n, &opComp, &opAttr);
		quickSort(array, p, q - 1, n);
		quickSort(array, q + 1, r, n);
	}
}


//QuickSort este un algoritm eficient pentru siruri de lungimi mari, dar in cazul sirurilor de lungimi mici, InsertionSort
//este mai eficient decat QuickSort
void quickSortHibridizat(int* array, int p, int r, int n, int prag, Operation* opComp, Operation* opAttr)
{
	//int gata = 0;
	while (p < r)
	{
		//daca lungimea sirului este mai mica decat 10, aplicam insertionSort si oprim recursivitatea
		if (r - p + 1 < prag)
		{
			insertionSort(array, p, r, opComp, opAttr);
			break;
		}
		else
		{
			int q = PARTITION(array, p, r, r - p + 1, opComp, opAttr);
			//daca partea de dinainte de pivot este mai mica decat partea de dupa, sortam partea stanga a sirului
			//si ne mutam pe partea dreapta apoi
			if (q - p < r - q)
			{
				quickSortHibridizat(array, p, q - 1, n, prag, opComp, opAttr);
				p = q + 1;
			}
			//daca partea de dupa pivot este mai mica decat partea de dinainte, sortam partea dreapta a sirului
			//si ne mutam pe partea stanga apoi
			else
			{
				quickSortHibridizat(array, q + 1, r, n, prag, opComp, opAttr);
				r = q - 1;
			}
		}
	}
}

void demoQuickSort()
{
	int n = 9;
	Operation opComp = pro.createOperation("QuickSortHibridizat-comp", n);
	Operation opAttr = pro.createOperation("QuickSortHibridizat-attr", n);
	int* array_1 = (int*)malloc(n * sizeof(int));
	FillRandomArray(array_1, n, 1, 100, false, 0);
	print_array(array_1, n);
	quickSortHibridizat(array_1, 0, 8, n, 10, &opComp, &opAttr);
	print_array(array_1, n);

	n = 15;
	Operation opComp1 = pro.createOperation("QuickSortHibridizat-comp", n);
	Operation opAttr1 = pro.createOperation("QuickSortHibridizat-attr", n);
	int* array_2 = (int*)malloc(n * sizeof(int));
	FillRandomArray(array_2, n, 1, 100, false, 0);
	print_array(array_2, n);
	quickSortHibridizat(array_2, 0, 14, n, 10, &opComp1, &opAttr1);
	print_array(array_2, n);
}

void save_array(int* array, int** saved_array, int n)
{
	for (int i = 0;i < n;i++)
		(*saved_array)[i] = array[i];
}

void quickSortOperatii()
{
	for (int n = STEPSIZE;n <= MAXSIZE;n += STEPSIZE)
	{
		for (int i = 0;i < NR_TESTS_1;i++)
		{
			Operation opComp = pro.createOperation("QuickSortHibridizat-comp", n);
			Operation opAttr = pro.createOperation("QuickSortHibridizat-attr", n);
			int* array = (int*)malloc(n * sizeof(int));
			if (array != NULL)
			{
				FillRandomArray(array, n, 1, n, false, 0);
				int* saved_array = (int*)malloc(n * sizeof(int));
				if (saved_array != NULL)
				{
					save_array(array, &saved_array, n);
					quickSort(array, 0, n - 1, n);
					quickSortHibridizat(saved_array, 0, n - 1, n, 10, &opComp, &opAttr);
				}
			}
		}
	}
	pro.divideValues("QuickSort-comp", NR_TESTS_1);
	pro.divideValues("QuickSort-attr", NR_TESTS_1);
	pro.divideValues("QuickSortHibridizat-comp", NR_TESTS_1);
	pro.divideValues("QuickSortHibridizat-attr", NR_TESTS_1);

	pro.addSeries("QuickSort", "QuickSort-comp", "QuickSort-attr");
	pro.addSeries("QuickSortHibridizat", "QuickSortHibridizat-comp", "QuickSortHibridizat-attr");

	pro.createGroup("QuickSort vs QuickSortHibridizat", "QuickSort", "QuickSortHibridizat");
	pro.showReport();
}

void timerQuickSort()
{
	quickSortOperatii();
	for (int n = STEPSIZE;n <= MAXSIZE;n += STEPSIZE)
	{
		pro.startTimer("quickSort", n);
		for (int test = 0; test < NR_TESTS_2; ++test)
		{
			int* array = (int*)malloc(n * sizeof(int));
			FillRandomArray(array, n, 1, n, false, 0);
			quickSort(array, 0, n - 1, n);
		}
		pro.stopTimer("quickSort", n);
	}

	for (int n = STEPSIZE;n <= MAXSIZE;n += STEPSIZE)
	{
		Operation opComp = pro.createOperation("QuickSortHibridizat-comp", n);
		Operation opAttr = pro.createOperation("QuickSortHibridizat-attr", n);
		pro.startTimer("quickSortHibridizat", n);
		for (int test = 0; test < NR_TESTS_2; ++test)
		{
			int* array = (int*)malloc(n * sizeof(int));
			FillRandomArray(array, n, 1, n, false, 0);
			quickSortHibridizat(array, 0, n - 1, n, 10, &opComp, &opAttr);
		}
		pro.stopTimer("quickSortHibridizat", n);
	}
	pro.createGroup("TIMER", "quickSort", "quickSortHibridizat");
	pro.showReport();
}

void pragQuickSort()
{
	int array[MAXSIZE];
	int *saved_array=(int*)malloc(MAXSIZE*sizeof(int));

	FillRandomArray(array, MAXSIZE, 1, MAXSIZE, true, 0);
	for (int i = 0;i <= 60;i++)
	{
		Operation opComp = pro.createOperation("QuickSortHibridizat-comp", i);
		Operation opAttr = pro.createOperation("QuickSortHibridizat-attr", i);
		save_array(array, &saved_array, MAXSIZE);
		quickSortHibridizat(saved_array, 0, MAXSIZE - 1, MAXSIZE, i, &opComp, &opAttr);
	}
	pro.addSeries("QuickSortHibridizat", "QuickSortHibridizat-comp", "QuickSortHibridizat-attr");
	pro.showReport();
}

int main()
{
	//demoParcurgeriArbore();
	//parcurgeriOperatii();
	//demoQuickSort();
	//quickSortOperatii();
	//timerQuickSort();
	//pragQuickSort();
	return 0;
}