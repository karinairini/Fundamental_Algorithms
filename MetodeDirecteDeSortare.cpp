/*
In cazul mediu statistic, Selection_Sort face cele mai putine atribuiri, adica c(n-1), unde c este aproape de 3, iar Bubble_Sort cele mai multe.
Insertion_Sort face cele mai putine comparatii, n(n-1)/2 (intodeauna face n(n-1)/2 comparatii deoarece nu depinde de date), iar Bubble_Sort cele mai multe.
Dupa interpretarea graficului cu suma celor doua operatii, Bubble_Sort este cel mai ineficient, iar Selection_Sort cel mai eficient, in cazul mediu statistic.

In cazul favorabil, Selection_Sort si Bubble_Sort fac 0 atribuiri, pe cand Insertion_Sort face 2(n-1).
Insertion_Sort si Bubble_Sort fac cele mai putine comparatii, n-1, iar Selection_Sort cele mai multe n(n-1)/2.
Dupa interpretarea graficului cu suma celor doua operatii, Bubble_Sort este cel mai eficient, iar Selection_Sort cel mai ineficient, in cazul favorabil.

In cazul nefavorabil, Bubble_Sort face cele mai multe atribuiri, iar Selection_Sort cele mai putine, 3(n-1). Insertion_Sort face 2(n-1)+n(n-1)/2 atribuiri.
Selection_Sort si Bubble_Sort fac acelasi numar de comparatii, acesta fiind cu putin sub numarul de comparatii pe care le face Insertion_Sort, n(n-1)/2.
Dupa interpretarea graficului cu suma celor doua operatii, Bubble_Sort este cel mai ineficient, iar Selection_Sort cel mai eficient, in cazul defavorabil.

Selection_Sort este adaptiv si instabil.
Insertion_Sort este adaptiv (mai adaptiv decat Selection_Sort) si stabil.
Bubble_sort este stabil, deoarece nu schimba pozitia elementelor egale, adica daca gaseste un element egal il pune dupa el.
*/

#include <stdlib.h>
#include <stdio.h>
#include "Profiler.h"

Profiler pro("MetodeDeSortare");

#define MAX_SIZE 5000
#define STEP_SIZE 100
#define NR_TESTS 5

void Selection_Sort(int array[], int n)
{
	Operation opComp = pro.createOperation("selection-comp", n);
	Operation opAttr = pro.createOperation("selection-attr", n);
	int index;
	for (int i = 0;i < n - 1;i++)
	{
		index = i;
		for (int j = i + 1;j < n;j++)
		{
			opComp.count();
			if (array[index] > array[j])
			{
				index = j;
			}
		}
		if (i != index)
		{
			opAttr.count(3);
			int aux = array[i];
			array[i] = array[index];
			array[index] = aux;
		}
	}
}

int cautare_binara(int array[], int key, int low, int high)
{
	if (high <= low)
	{
		if (array[low] < key)
		{
			return low + 1;
		}
		else
		{
			return low;
		}
	}
	int mijloc = (low + high) / 2;
	if (key == array[mijloc])
	{
		return mijloc + 1;
	}
	if (key < array[mijloc])
	{
		return cautare_binara(array, key, low, mijloc - 1);
	}
	else
	{
		return cautare_binara(array, key, mijloc + 1, high);
	}
}

void Insertion_Sort_Binary(int array[], int n)
{
	Operation opComp = pro.createOperation("insertion_binary-comp", n);
	Operation opAttr = pro.createOperation("insertion_binary-attr", n);
	for (int i = 1;i < n;i++)
	{
		opAttr.count();
		int key = array[i];
		int j = i - 1;
		//gasesc indexul unde trebuie inserat elementul
		int index = cautare_binara(array, key, 0, j);
		//mut elementele la dreapta ca sa fac loc
		while (j >= 0 && j >= index)
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

void Insertion_Sort(int array[], int n)
{
	Operation opComp = pro.createOperation("insertion-comp", n);
	Operation opAttr = pro.createOperation("insertion-attr", n);
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

void Bubble_Sort(int array[], int n)
{
	Operation opComp = pro.createOperation("bubble-comp", n);
	Operation opAttr = pro.createOperation("bubble-attr", n);
	for (int i = 0;i < n - 1;i++)
	{
		//variabila swapped ne ajuta sa vedem daca s-a facut vreo interschimbare, daca nu s-a facut inseamna ca vectorul este gata sortat
		int swapped = 0;
		for (int j = 0;j < n - i - 1;j++)
		{
			opComp.count();
			if (array[j] > array[j + 1])
			{
				opAttr.count(3);
				int aux = array[j];
				array[j] = array[j + 1];
				array[j + 1] = aux;
				swapped = 1;
			}
		}
		if (swapped == 0)
			break;
	}
}

void print_array(int array[], int n)
{
	for (int i = 0;i < n;i++)
		printf("%d ", array[i]);
}

//avem nevoie sa ne salvam vectorul pentru a face toate cele 3 sortari, altfel dupa prima sortare vectorul este sortat
void save_array(int array[], int n, int saved_array[])
{
	for (int i = 0;i < n;i++)
	{
		saved_array[i] = array[i];
	}
}

void average_case()
{
	int array[MAX_SIZE];
	int saved_array[MAX_SIZE];
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		for (int i = 0;i < NR_TESTS;i++)
		{
			FillRandomArray(array, n, 10, 50000, false, 0);
			save_array(array, n, saved_array);
			Selection_Sort(saved_array, n);
			save_array(array, n, saved_array);
			Insertion_Sort(saved_array, n);
			Bubble_Sort(array, n);
		}
	}
	pro.divideValues("selection-comp", NR_TESTS);
	pro.divideValues("selection-attr", NR_TESTS);
	pro.addSeries("selection_sort", "selection-comp", "selection-attr");

	pro.divideValues("insertion-comp", NR_TESTS);
	pro.divideValues("insertion-attr", NR_TESTS);
	pro.addSeries("insertion_sort", "insertion-comp", "insertion-attr");

	pro.divideValues("bubble-comp", NR_TESTS);
	pro.divideValues("bubble-attr", NR_TESTS);
	pro.addSeries("bubble_sort", "bubble-comp", "bubble-attr");

	//deoarece graficul pentru selection-attr nu se vede bine, am ales sa il afisez separat
	pro.createGroup("Average-comp", "selection-comp", "insertion-comp", "bubble-comp");
	pro.createGroup("Average-attr", "insertion-attr", "bubble-attr");
	pro.createGroup("Average-case-sum", "selection_sort", "insertion_sort", "bubble_sort");

	pro.showReport();
}

void best_case()
{
	//cazul favorabil pentru toate cele 3 sortari se intampla atunci cand vectorul este crescator
	int array[MAX_SIZE];
	int saved_array[MAX_SIZE];
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		FillRandomArray(array, n, 10, 50000, false, ASCENDING);
		save_array(array, n, saved_array);
		Selection_Sort(saved_array, n);
		save_array(array, n, saved_array);
		Insertion_Sort(saved_array, n);
		Bubble_Sort(array, n);
	}
	pro.addSeries("selection_sort", "selection-comp", "selection-attr");

	pro.addSeries("insertion_sort", "insertion-comp", "insertion-attr");

	pro.addSeries("bubble_sort", "bubble-comp", "bubble-attr");

	pro.createGroup("Best-comp", "selection-comp", "insertion-comp", "bubble-comp");
	pro.createGroup("Best-attr", "selection-attr", "insertion-attr", "bubble-attr");
	//deoarece deoarece graficul pentru insertion-sort nu se vede bine, am ales sa il afisez separat
	pro.createGroup("Best-case-sum", "selection_sort", "bubble_sort");

	pro.showReport();
}

void worst_case()
{
	//cazul defavorabil pentru toate cele 3 sortari se intampla atunci cand vectorul este descrescator
	int array[MAX_SIZE];
	int saved_array[MAX_SIZE];
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		FillRandomArray(array, n, 10, 50000, false, DESCENDING);
		save_array(array, n, saved_array);
		Selection_Sort(saved_array, n);
		save_array(array, n, saved_array);
		Insertion_Sort(saved_array, n);
		Bubble_Sort(array, n);
	}
	pro.addSeries("selection_sort", "selection-comp", "selection-attr");

	pro.addSeries("insertion_sort", "insertion-comp", "insertion-attr");

	pro.addSeries("bubble_sort", "bubble-comp", "bubble-attr");

	pro.createGroup("Worst-comp", "selection-comp", "insertion-comp", "bubble-comp");
	pro.createGroup("Worst-attr", "selection-attr", "insertion-attr", "bubble-attr");
	pro.createGroup("Worst-case-sum", "selection_sort", "insertion_sort", "bubble_sort");

	pro.showReport();
}

int main()
{
	//am demonstrat corectitudinea algoritmilor pe un vector de dimensiune mica
	/*
	int array_for_demonstration[] = {5, 1, 4, 2, 8};
	int n = sizeof(array_for_demonstration) / sizeof(int);
	//Selection_Sort(array_for_demonstration, n);
	//Insertion_Sort(array_for_demonstration, n);
	//Insertion_Sort_Binary(array_for_demonstration, n);
	//Bubble_Sort(array_for_demonstration, n);
	printf("Vectorul sortat: ");
	print_array(array_for_demonstration, n);
	printf("\n");
	*/
	//average_case();
	//best_case();
	worst_case();
	return 0;
}