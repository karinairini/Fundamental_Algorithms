/*
Din graficele generate, putem observa ca algoritmul este mai eficient pentru un numar de liste (k) mai mic.
Complexitatea algoritmului de interclasa a k liste este O(n*logk).
Algoritmul de creare a heap-ului cu metoda Bottom-up are complexitatea O(k), iar algoritmul MIN_HEAPIFY 
are complexitatea O(lgk).
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Profiler.h"

Profiler pro("Interclasare_k_liste");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

typedef struct node_type
{
	int key;
	struct node_type* next;
}Node;

typedef struct list_header
{
	Node* first;
	Node* last;
}List;

void createEmpty(List* l)
{
	//initializam lista
	l->first = NULL;
	l->last = NULL;
}

Node* createNode(int key)
{
	//alocam memorie
	Node* node = (Node*)malloc(sizeof(Node));
	if (node != NULL)
	{
		//copiam cheia in nodul pointat de catre node
		node->key = key;
		//initializam nodul urmator catre NULL
		node->next = NULL;
	}
	return node;
}

void insertLast(List* l, int key)
{
	Node* node = createNode(key);
	//daca lista este vida, acest nod va fi singur in lista
	if (l->first == NULL)
	{
		l->first = node;
		l->last = node;
	}
	else
	{
		l->last->next = node;
		l->last = node;
	}
}

void insertLast_comp(List* l, int key, Operation* opComp, Operation* opAttr)
{
	Node* node = createNode(key);
	//daca lista este vida, acest nod va fi singur in lista
	opComp->count();
	if (l->first == NULL)
	{
		opAttr->count(2);
		l->first = node;
		l->last = node;
	}
	else
	{
		opAttr->count(2);
		l->last->next = node;
		l->last = node;
	}
}

void printList(List l)
{
	while (l.first != NULL)
	{
		printf("%d ", (l.first)->key);
		l.first = (l.first)->next;
	}
	printf("\n");
}

void print_array(int array[], int k)
{
	for (int i = 0;i < k;i++)
		printf("%d ", array[i]);
	printf("\n");
}

void randomList(int k, int n, List* l)
{
	int* array_nr_elements = (int*)malloc(k * sizeof(int));
	if (array_nr_elements != NULL)
	{
		//initializez toate elementele cu 1 pentru a nu risca ca vreo lista sa aiba 0 elemente
		for (int i = 0;i < k;i++)
			array_nr_elements[i] = 1;
		srand(time(0));
		//generez random numarul de elemente din fiecare lista
		for (int i = k;i < n;i++)
		{
			array_nr_elements[(rand() % k)]++;
		}
		//print_array(array_nr_elements, k);
		for (int i = 0;i < k;i++)
		{
			//generez elemenetele pentru fiecare lista ordonata
			int* array_elements = (int*)malloc(array_nr_elements[i] * sizeof(int));
			if (array_elements != NULL)
			{
				//incarc elemenetele in lista
				createEmpty(&l[i]);
				FillRandomArray(array_elements, array_nr_elements[i], 10, 50000, false, ASCENDING);
				for (int j = 0;j < array_nr_elements[i];j++)
				{
					insertLast(&l[i], array_elements[j]);
				}
			}
		}
	}
}

void MIN_HEAPIFY(List** array_list, int n, int i, Operation* opComp, Operation* opAttr)
{
	int smallest;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	opComp->count();
	if (left < n &&  (*array_list[left]).first->key < (*array_list[i]).first->key)
	{
		smallest = left;
	}
	else
	{
		smallest = i;
	}
	opComp->count();
	if (right < n && (*array_list[right]).first->key < (*array_list[smallest]).first->key)
	{
		smallest = right;
	}
	if (smallest != i)
	{
		opAttr->count(3);
		List aux = (*array_list[i]);
		(*array_list[i]) = (*array_list[smallest]);
		(*array_list[smallest]) = aux;
		MIN_HEAPIFY(array_list, n, smallest, opComp, opAttr);
	}
}

void BUILD_HEAP(List** array_list, int n, Operation* opComp, Operation* opAttr)
{
	int length = (n / 2) - 1;
	for (int i = length;i >= 0;i--)
		MIN_HEAPIFY(array_list, n, i, opComp, opAttr);
}

void kwayMerge(List** array_list, int k, List* Lout, Operation* opComp, Operation* opAttr)
{
	int heapsize = k;
	createEmpty(Lout);
	BUILD_HEAP(array_list, k, opComp, opAttr);
	while (heapsize > 0)
	{
		insertLast_comp(Lout, (*array_list[0]).first->key, opComp, opAttr);
		opAttr->count();
		(*array_list[0]).first = (*array_list[0]).first->next;
		opComp->count();
		if ((*array_list[0]).first == NULL)
		{
			opAttr->count();
			(*array_list[0]) = (*array_list[heapsize-1]);
			heapsize = heapsize - 1;
		}
		MIN_HEAPIFY(array_list, heapsize, 0, opComp, opAttr);
	}
}

void average_case_n_variabil()
{
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		List Lout;
		Operation opComp1 = pro.createOperation("Kway-comp_k_5", n);
		Operation opAttr1 = pro.createOperation("Kway-attr_k_5", n);

		Operation opComp2 = pro.createOperation("Kway-comp_k_10", n);
		Operation opAttr2 = pro.createOperation("Kway-attr_k_10", n);

		Operation opComp3 = pro.createOperation("Kway-comp_k_100", n);
		Operation opAttr3 = pro.createOperation("Kway-attr_k_100", n);

		int k = 5;
		List* l = (List*)malloc(k * sizeof(List));
		List** array_list = (List**)malloc(k * sizeof(List*));
		for (int i = 0;i < k;i++)
			array_list[i] = (List*)malloc(i * sizeof(List));
		randomList(k, n, l);
		for(int i = 0;i < k;i++)
			array_list[i] = &l[i];

		kwayMerge(array_list, k, &Lout, &opComp1, &opAttr1);

		free(array_list);
		free(l);

		k = 10;
		l = (List*)malloc(k * sizeof(List));
		array_list = (List**)malloc(k * sizeof(List*));
		for (int i = 0;i < k;i++)
			array_list[i] = (List*)malloc(i * sizeof(List));
		randomList(k, n, l);
		for (int i = 0;i < k;i++)
			array_list[i] = &l[i];

		kwayMerge(array_list, k, &Lout, &opComp2, &opAttr2);

		free(array_list);
		free(l);

		k = 100;
		l = (List*)malloc(k * sizeof(List));
		array_list = (List**)malloc(k * sizeof(List*));
		for (int i = 0;i < k;i++)
			array_list[i] = (List*)malloc(i * sizeof(List));
		randomList(k, n, l);
		for (int i = 0;i < k;i++)
			array_list[i] = &l[i];

		kwayMerge(array_list, k, &Lout, &opComp3, &opAttr3);

		free(array_list);
		free(l);
	}
	pro.addSeries("TOTAL-Kway_k_5", "Kway-comp_k_5", "Kway-attr_k_5");
	pro.addSeries("TOTAL-Kway_k_10", "Kway-comp_k_10", "Kway-attr_k_10");
	pro.addSeries("TOTAL-Kway_k_100", "Kway-comp_k_100", "Kway-attr_k_100");

	pro.createGroup("TOTAL_OPERATIONS_Kway", "TOTAL-Kway_k_5", "TOTAL-Kway_k_10", "TOTAL-Kway_k_100");
	pro.createGroup("TOTAL_COMP_Kway", "Kway-comp_k_5", "Kway-comp_k_10", "Kway-comp_k_100");
	pro.createGroup("TOTAL_ATTR_Kway", "Kway-attr_k_5", "Kway-attr_k_10", "Kway-attr_k_100");

	pro.showReport();
}

void average_case_k_variabil()
{
	for (int k = 10;k <= 500;k += 10)
	{
		Operation opComp = pro.createOperation("Kway-comp", k);
		Operation opAttr = pro.createOperation("Kway-attr", k);

		List Lout;
		List* l = (List*)malloc(k * sizeof(List));
		List** array_list = (List**)malloc(k * sizeof(List*));
		for (int i = 0;i < k;i++)
			array_list[i] = (List*)malloc(i * sizeof(List));

		randomList(k, MAX_SIZE, l);

		for (int i = 0;i < k;i++)
			array_list[i] = &l[i];

		kwayMerge(array_list, k, &Lout, &opComp, &opAttr);

		free(l);
		free(array_list);
	}
	pro.addSeries("TOTAL-Kway", "Kway-comp", "Kway-attr");

	pro.showReport();
}

int main()
{
	int n = 20;
	int k = 4;
	Operation opComp = pro.createOperation("Kway-comp", n);
	Operation opAttr = pro.createOperation("Kway-attr", n);
	List* l = (List*)malloc(k * sizeof(List));
	List** array_list = (List**)malloc(k * sizeof(List*));
	if (array_list != NULL)
	{
		for (int i = 0;i < k;i++)
			array_list[i] = (List*)malloc(i * sizeof(List));
		if (l != NULL)
		{
			randomList(k, n, l);

			for (int i = 0;i < k;i++)
			{
				printf("Lista %d: ", i + 1);
				printList(l[i]);
			}

			for (int i = 0;i < k;i++)
				array_list[i] = &l[i];

			List Lout;
			kwayMerge(array_list, k, &Lout, &opComp, &opAttr);
			printf("Listele interclasate: ");
			printList(Lout);
		}
	}
	
	//average_case_n_variabil();
	//average_case_k_variabil();
	return 0;
}