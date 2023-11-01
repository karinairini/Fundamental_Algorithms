/*
Prin interpretarea graficelor generate in urma codului de mai jos, am observat ca
metoda de construire BOTTOM-UP a structurii de date heap are complexitatea O(n), iar cea TOP_DOWN O(nlgn).

Avantajul metodei BOTTOM-UP este ca e mai rapida, pe cand cea TOP-DOWN este potrivit pentru dimensiuni variabile.
Metoda BOTTOM-UP este mai eficienta decat cea TOP-DOWN.

Metoda de sortare HEAPSORT are complexitatea O(nlgn) si este un algoritm optim.
*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "Profiler.h"

Profiler pro("BuildHeap");

#define MAX_SIZE 5000
#define STEP_SIZE 100
#define NR_TESTS 5

void MAX_HEAPIFY(int array[], int n, int i)
{
	Operation opComp = pro.createOperation("BOTTOM_UP-comp", n);
	Operation opAttr = pro.createOperation("BOTTOM_UP-attr", n);
	int largest; 
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	opComp.count();
	if (left < n && array[left] > array[i])
	{
		largest = left;
	}
	else
	{
		largest = i;
	}
	opComp.count();
	if (right < n && array[right] > array[largest])
	{
		largest = right;
	}
	if (largest != i)
	{
		opAttr.count(3);
		int aux = array[i];
		array[i] = array[largest];
		array[largest] = aux;
		MAX_HEAPIFY(array, n, largest);
	}
}

void BOTTOM_UP(int array[], int n)
{
	int length = (n / 2) - 1;
	for (int i = length;i >= 0;i--)
		MAX_HEAPIFY(array, n, i);
}

void print_heap(int array[], int n)
{
	for (int i = 0;i < n;i++)
		printf("%d ", array[i]);
	printf("\n");
}

void HEAPSORT(int array[], int n)
{
	BOTTOM_UP(array, n);
	for (int i = n - 1; i >= 1;i--)
	{	
		int aux = array[i];
		array[i] = array[0];
		array[0] = aux;
		MAX_HEAPIFY(array, i , 0);
	}
}

void save_array(int array[], int n, int saved_array[])
{
	for (int i = 0;i < n;i++)
		saved_array[i] = array[i];
}

void HEAP_INCREASE_KEY(int array[], int i, int key, Operation* opComp, Operation* opAttr)
{
	opComp->count();
	if (key < array[i])
		return;
	opAttr->count();
	array[i] = key;
	while (i > 0 && array[(i-1) / 2] < array[i])
	{
		opComp->count();
		opAttr->count(3);
		int aux = array[(i-1) / 2];
		array[(i - 1) / 2] = array[i];
		array[i] = aux;
		i = (i-1) / 2;
	}
	opComp->count();
}


void MAX_HEAP_INSERT(int array[], int* n, int key, Operation* opComp, Operation* opAttr)
{
	(* n) = (* n) + 1;
	opAttr->count();
	array[(* n) - 1] = INT_MIN;
	HEAP_INCREASE_KEY(array, (* n) - 1, key, opComp, opAttr);
}

void TOP_DOWN(int array[], int n, int array2[], int* m)
{
	Operation opComp = pro.createOperation("TOP_DOWN-comp", n);
	Operation opAttr = pro.createOperation("TOP_DOWN-attr", n);
	(* m) = 0;
	for (int i = 0;i < n;i++)
		MAX_HEAP_INSERT(array2, m, array[i], &opComp, &opAttr);
}

void average_case()
{
	int array[MAX_SIZE];
	int saved_array[MAX_SIZE];
	int top_down_array[MAX_SIZE];
	int m = 0;
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		for (int i = 0;i < NR_TESTS;i++)
		{
			FillRandomArray(array, n, 10, 50000, false, 0);
			save_array(array, n, saved_array);
			BOTTOM_UP(saved_array, n);
			TOP_DOWN(array, n, top_down_array,&m);
		}
	}
	pro.divideValues("BOTTOM_UP-comp", NR_TESTS);
	pro.divideValues("BOTTOM_UP-attr", NR_TESTS);
	pro.addSeries("BOTTOM_UP", "BOTTOM_UP-comp", "BOTTOM_UP-attr");

	pro.divideValues("TOP_DOWN-comp", NR_TESTS);
	pro.divideValues("TOP_DOWN-attr", NR_TESTS);
	pro.addSeries("TOP_DOWN", "TOP_DOWN-comp", "TOP_DOWN-attr");

	pro.createGroup("TOTAL_AVG", "BOTTOM_UP", "TOP_DOWN");
	pro.createGroup("TOTAL_AVG-COMP", "BOTTOM_UP-comp", "TOP_DOWN-comp");
	pro.createGroup("TOTAL_AVG-ATTR", "BOTTOM_UP-attr", "TOP_DOWN-attr");

	pro.showReport();
}

void worst_case()
{
	int array[MAX_SIZE];
	int saved_array[MAX_SIZE];
	int top_down_array[MAX_SIZE];
	int m = 0;
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		for (int i = 0;i < NR_TESTS;i++)
		{
			FillRandomArray(array, n, 10, 50000, false, ASCENDING);
			save_array(array, n, saved_array);
			BOTTOM_UP(saved_array, n);
			TOP_DOWN(array, n, top_down_array, &m);
		}
	}
	pro.divideValues("BOTTOM_UP-comp", NR_TESTS);
	pro.divideValues("BOTTOM_UP-attr", NR_TESTS);
	pro.addSeries("BOTTOM_UP", "BOTTOM_UP-comp", "BOTTOM_UP-attr");

	pro.divideValues("TOP_DOWN-comp", NR_TESTS);
	pro.divideValues("TOP_DOWN-attr", NR_TESTS);
	pro.addSeries("TOP_DOWN", "TOP_DOWN-comp", "TOP_DOWN-attr");

	pro.createGroup("TOTAL_WORST", "BOTTOM_UP", "TOP_DOWN");
	pro.createGroup("TOTAL_WORST-COMP", "BOTTOM_UP-comp", "TOP_DOWN-comp");
	pro.createGroup("TOTAL_WORST-ATTR", "BOTTOM_UP-attr", "TOP_DOWN-attr");

	pro.showReport();
}

int main()
{
	int array[] = {1, 3, 5, 4, 6, 13, 10, 9, 8, 15, 17};
	int n = sizeof(array) / sizeof(int);
	
	int* saved_array = (int*)malloc(n * sizeof(int));
	save_array(array, n, saved_array);

	BOTTOM_UP(saved_array, n);
	print_heap(saved_array, n);

	save_array(array, n, saved_array);

	HEAPSORT(saved_array, n);
	printf("Vectorul sortat: ");
	print_heap(saved_array, n);
	
	int array2[11] = {0};
	int m = 0;
	TOP_DOWN(array, n, array2, &m);
	print_heap(array2, m);
	
	//average_case();
	//worst_case();
	return 0;
}