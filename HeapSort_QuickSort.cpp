/*
În cazul favorabil, se poate observa din graficele generate faptul că algoritmul QuickSort are complexitate O(nlgn).
Cazul mediu statistic, se poate demonstra ca este apropiat de cazul favorabil, observându-se acest aspect și din graficele 
generate. Algortimul QuickSort are, în cazul mediu statistic, complexitatea O(nlgn).
În cazul nefavorabil, se poate observa din graficele generate faptul că algoritmul QuickSort are complexitate O(n^2).

După generarea graficelor pentru compararea celor două metode de sortare, se poate observa că algoritmul QuickSort este mai 
eficient decât algoritmul HeapSort.

Ambele implementări ale algoritmului InsertionSort, cea iterativă respectiv cea recursivă, au complexitatea O(n^2). Prima variantă
are nevoie de O(1) spațiu auxiliar, iar cea de-a doua O(n) spațiu auxiliar. InsertionSort recursiv are timpul de rulare mai mare
decât InsertionSort iterativ.
*/
#include <stdlib.h>
#include <stdio.h>
#include "Profiler.h"

Profiler pro("Construire_HeapSort_QuickSort");

#define MAX_SIZE 4000
#define STEP_SIZE 100
#define NR_TESTS 5
#define NR_TESTS_2 100

/*
Functia PARTITION ia ca și pivot ultimul element, îl plasează în poziția 
corectă în șirul sortat și mută elementele mai mici decât pivotul, la stânga
lui, iar elementele mai mari la dreapta lui.
*/
int PARTITION(int array[], int p, int r, int n)
{
	//Lomuto partition - O(n^2)
	Operation opComp = pro.createOperation("QuickSort-comp", n);
	Operation opAttr = pro.createOperation("QuickSort-attr", n);
	opAttr.count();
	//x - pivotul este ultimul element al sirului
	int x = array[r];
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		opComp.count();
		//daca elementul curent este mai mic sau egal cu pivotul
		if (array[j] <= x)
		{
			i++; //crește indexul elementului mai mic
			opAttr.count(3);
			int aux_1 = array[i];
			array[i] = array[j];
			array[j] = aux_1;
		}
	}
	opAttr.count(3);
	int aux_2 = array[i + 1];
	array[i + 1] = array[r];
	array[r] = aux_2;
	return i + 1;
}

void QUICKSORT(int array[], int p, int r, int n)
{
	//sorteaza prima parte pana unde s-a inserat pivotul si dupa partea de dupa pivot
	if (p < r)
	{
		int q = PARTITION(array, p, r, n);
		QUICKSORT(array, p, q - 1, n);
		QUICKSORT(array, q + 1, r, n);
	}
}

void print_array(int array[], int n)
{
	for (int i = 0; i < n; i++)
		printf("%d ", array[i]);
	printf("\n");
}

void average_case_QuickSort()
{
	int array[MAX_SIZE];
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		for (int j = 0;j < NR_TESTS;j++)
		{
			//șir nesortat pentru cazul mediu statistic
			FillRandomArray(array, n);
			QUICKSORT(array, 0, n - 1, n);
		}
	}
	pro.divideValues("QuickSort-comp", NR_TESTS);
	pro.divideValues("QuickSort-attr", NR_TESTS);
	pro.addSeries("QuickSort-average", "QuickSort-comp", "QuickSort-attr");

	pro.showReport();
}

void best_case_QuickSort()
{
	int array[MAX_SIZE];
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		/*
		cazul favorabil apare atunci când partițiile au lungimi egale ori cu 1 mai mic, 
		când pivotul se află în mijloc și fiecare partiție are(n - 1) / 2 elemente
		*/
		FillRandomArray(array, n);
		QUICKSORT(array, 0, (n - 1) / 2, n);
	}
	pro.addSeries("QuickSort-best", "QuickSort-comp", "QuickSort-attr");

	pro.showReport();
}

void worst_case_QuickSort()
{
	int array[MAX_SIZE];
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		//șir crescător pentru cazul nefavorabil
		FillRandomArray(array, n, 10, 50000, false, 1);
		QUICKSORT(array, 0, n - 1, n);
	}
	pro.addSeries("QuickSort-worst", "QuickSort-comp", "QuickSort-attr");

	pro.showReport();
}

void MAX_HEAPIFY(int array[], int n, int i, Operation* opComp, Operation* opAttr)
{
	//verifica daca un copil este mai mare decat parintele, daca este ii interschimba si continua sa 
	//verifice in continuarea nodului cu care am interschimbat
	int largest;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	opComp->count();
	if (left < n && array[left] > array[i])
	{
		largest = left;
	}
	else
	{
		largest = i;
	}
	opComp->count();
	if (right < n && array[right] > array[largest])
	{
		largest = right;
	}
	if (largest != i)
	{
		opAttr->count(3);
		int aux = array[i];
		array[i] = array[largest];
		array[largest] = aux;
		MAX_HEAPIFY(array, n, largest, opComp, opAttr);
	}
}

void HEAPSORT(int array[], int n)
{
	Operation opComp = pro.createOperation("HeapSort-comp", n);
	Operation opAttr = pro.createOperation("HeapSort-attr", n);
	//construiesc heap-ul BOTTOM-UP
	//verifica prima jumatate de elemente, iar daca interschimba, merge mai jos pe ramura si verifica
	int length = (n / 2) - 1;
	for (int i = length;i >= 0;i--)
		MAX_HEAPIFY(array, n, i, &opComp, &opAttr);
	//primul element din array este cel mai mare
	for (int i = n - 1;i >= 1;i--)
	{
		//interschimba primul element cu ultimul curent, astfel ultimul element va fi cel mai mare de pana acum
		opAttr.count(3);
		int aux = array[i];
		array[i] = array[0];
		array[0] = aux;
		//aduce elementul cel mai mare pe prima pozitie
		MAX_HEAPIFY(array, i, 0, &opComp, &opAttr);
	}
}

void save_array(int array[], int n, int saved_array[])
{
	for (int i = 0;i < n;i++)
		saved_array[i] = array[i];
}

void average_case_HeapSort_QuickSort()
{
	int array[MAX_SIZE];
	int saved_array[MAX_SIZE];
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		for (int j = 0;j < NR_TESTS;j++)
		{
			FillRandomArray(array, n);
			//avem nevoie sa ne salvăm șirul, altfel după QuickSort șirul este sortat, iar HeapSort nu va fi în cazul mediu statistic, ci în cazul nefavorabil
			save_array(array, n, saved_array);
			QUICKSORT(array, 0, n - 1, n);
			HEAPSORT(saved_array, n);
		}
	}
	pro.divideValues("QuickSort-comp", NR_TESTS);
	pro.divideValues("QuickSort-attr", NR_TESTS);
	pro.addSeries("QuickSort-average", "QuickSort-comp", "QuickSort-attr");

	pro.divideValues("HeapSort-comp", NR_TESTS);
	pro.divideValues("HeapSort-attr", NR_TESTS);
	pro.addSeries("HeapSort-average", "HeapSort-comp", "HeapSort-attr");

	pro.createGroup("Average-comp", "QuickSort-comp", "HeapSort-comp");
	pro.createGroup("Average-attr", "QuickSort-attr", "HeapSort-attr");
	pro.createGroup("Average-case_opperations", "QuickSort-average", "HeapSort-average");

	pro.showReport();
}

void Insertion_Sort(int array[], int n)
{
	Operation opComp = pro.createOperation("Insertion-comp", n);
	Operation opAttr = pro.createOperation("Insertion-attr", n);
	for (int i = 1;i < n;i++)
	{
		opAttr.count();
		int key = array[i];
		int j = i - 1;
		while (j >= 0 && array[j] > key)
		{
			opComp.count();
			opAttr.count();
			array[j + 1] = array[j];
			j = j - 1;
		}
		opComp.count();
		opAttr.count();
		array[j + 1] = key;
	}
}
//Insertion Sort tine elementele deja procesate care sunt sortate si insereaza elementele unul cate unul 
//in sirul sortat
void Insertion_Sort_recursiv(int array[], int n)
{
	Operation opComp = pro.createOperation("Insertion_recursiv-comp", n);
	Operation opAttr = pro.createOperation("Insertion_recursiv-attr", n);
	if (n <= 1)
		return;
	//sortăm primele n-1 elemente
	Insertion_Sort_recursiv(array, n - 1);

	//inserăm ultimul element în poziția corectă din șirul sortat
	opAttr.count();
	int last_elem = array[n - 1];
	int j = n - 2;

	while (j >= 0 && array[j] > last_elem)
	{
		opComp.count();
		opAttr.count();
		array[j + 1] = array[j];
		j--;
	}
	opComp.count();
	opAttr.count();
	array[j + 1] = last_elem;
}

void Insertion_Sort_Timer()
{
	int array[MAX_SIZE];
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS_2; ++test)
		{
			FillRandomArray(array, n);
			Insertion_Sort(array, n);
		}
	}
	pro.divideValues("Insertion-comp", NR_TESTS_2);
	pro.divideValues("Insertion-attr", NR_TESTS_2);

	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		pro.startTimer("Insertion_Sort", n);
		for (int test = 0; test < NR_TESTS_2; ++test) 
		{
			FillRandomArray(array, n);
			Insertion_Sort(array, n);
		}	
		pro.stopTimer("Insertion_Sort", n);
	}
	pro.showReport();
}	

void Insertion_Sort_recursiv_Timer()
{
	int array[MAX_SIZE];
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS_2; ++test)
		{
			FillRandomArray(array, n);
			Insertion_Sort_recursiv(array, n);
		}
	}
	pro.divideValues("Insertion_recursiv-comp", NR_TESTS_2);
	pro.divideValues("Insertion_recursiv-attr", NR_TESTS_2);

	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		pro.startTimer("Insertion_Sort_recursiv", n);
		for (int test = 0; test < NR_TESTS_2; ++test)
		{
			FillRandomArray(array, n);
			Insertion_Sort_recursiv(array, n);
		}
		pro.stopTimer("Insertion_Sort_recursiv", n);
	}
	pro.showReport();
}

void average_case_Insertion_all()
{
	int array[MAX_SIZE];
	int saved_array[MAX_SIZE];
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS_2; ++test)
		{
			FillRandomArray(array, n);
			//avem nevoie sa ne salvăm șirul, altfel după Insertion_Sort șirul este sortat
			save_array(array, n, saved_array);
			Insertion_Sort(array, n);
			Insertion_Sort_recursiv(saved_array, n);
		}
	}
	pro.divideValues("Insertion-comp", NR_TESTS_2);
	pro.divideValues("Insertion-attr", NR_TESTS_2);
	pro.addSeries("Insertion_Sort", "Insertion-comp", "Insertion-attr");

	pro.divideValues("Insertion_recursiv-comp", NR_TESTS_2);
	pro.divideValues("Insertion_recursiv-attr", NR_TESTS_2);
	pro.addSeries("Insertion_Sort_recursiv", "Insertion_recursiv-comp", "Insertion_recursiv-attr");

	//deoarece nu se pot vedea graficele aferente Insertion_Sort_recursiv, le-am afișat separat
	pro.createGroup("Insertion_Sort_all_comp", "Insertion-comp");
	pro.createGroup("Insertion_Sort_all_attr", "Insertion-attr");
	pro.createGroup("InsertionSort_all-opperations", "Insertion_Sort");
		
	pro.showReport();
}

int RANDOMIZED_PARTITION(int array[], int p, int r, int n)
{
	int i = p + rand() % (r - p); //indexul unde sa inseram ultimul element
	int aux = array[r];
	array[r] = array[i];
	array[i] = aux;
	return PARTITION(array, p, r, n);
}

//returneaza al k-lea cel mai mic element din sir
int RANDOMIZED_SELECT(int array[], int p, int r, int i, int n)
{
	if (p == r)
		return array[p];
	int q = RANDOMIZED_PARTITION(array, p, r, n);
	int k = q - p;
	if (i == k)
		return array[q];
	else if (i < k)
		return RANDOMIZED_SELECT(array, p, q - 1, i, n);
	else return RANDOMIZED_SELECT(array, q + 1, r, i - k, n);	
}

void QUICKSORT_RANDOM(int array[], int p, int r, int n)
{
	if (p < r)
	{
		int q = RANDOMIZED_SELECT(array, p, r, 0, n);
		QUICKSORT(array, p, q - 1, n);
		QUICKSORT(array, q + 1, r, n);
	}
}

int main()
{
	/*
	int array[] = { 10, 7, 8, 9, 1, 5 };
	int n = sizeof(array) / sizeof(int);
	
	int* saved_array = (int*)malloc(n * sizeof(int));
	save_array(array, n, saved_array);

	QUICKSORT(saved_array, 0, n - 1, n);
	printf("Vectorul sortat prin QuickSort: \n");
	print_array(saved_array, n);

	save_array(array, n, saved_array);
	HEAPSORT(saved_array, n);
	printf("Vectorul sortat prin HeapSort: \n");
	print_array(saved_array, n);

	save_array(array, n, saved_array);
	Insertion_Sort(saved_array, n);
	printf("Vectorul sortat prin InsertionSort: \n");
	print_array(saved_array, n);

	save_array(array, n, saved_array);
	Insertion_Sort_recursiv(saved_array, n);
	printf("Vectorul sortat prin InsertionSort recursiv: \n");
	print_array(saved_array, n);

	QUICKSORT_RANDOM(array, 0, n - 1, n);
	printf("Vectorul sortat prin QuickSort implementat cu Randomized-Select: \n");
	print_array(array, n);
	*/

	//average_case_QuickSort();
	//best_case_QuickSort();
	//worst_case_QuickSort();

	//average_case_HeapSort_QuickSort();

	//Insertion_Sort_Timer();
	//Insertion_Sort_recursiv_Timer();
	//average_case_Insertion_all();
	
	return 0;
}