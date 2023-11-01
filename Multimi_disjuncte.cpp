//Algoritmul lui Kruskal are complexitate O(MlogN), unde N-nr_noduri, M-nr_muchii
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler pro("Multimi_disjuncte");

#define STEP_SIZE 100
#define MAX_SIZE 10000

//structura pastreaza informatii despre cheia nodului, pointer catre parintele sau si rank-ul
typedef struct Node_Set
{
	int key;
	struct Node_Set* parinte;
	int* rank; //reprezinta inaltimea arborelui, numarul de muchii din care este format cel mai lung drum de la nod la o frunza
}Node_Set;

typedef struct muchie
{
	Node_Set* nod1, * nod2;
	int cost;
}Muchie;

typedef struct _GRAF_INFO
{
	Node_Set** V; //multimea de noduri din graf
	Muchie** E; //multimea de muchii din graf
}Graf;

//initializeaza un arbore care este format doar dintr-un nod care are rank-ul 0 si pointeaza catre el insusi, cu cheia dorita key
Node_Set* MAKESET(int key)
{
	Node_Set* new_node = (Node_Set*)malloc(sizeof(Node_Set));
	if (new_node != NULL)
	{
		new_node->key = key;
		new_node->parinte = new_node;
		new_node->rank = 0;
	}
	return new_node;
}

//initializeaza un arbore care este format doar dintr-un nod care are rank-ul 0 si pointeaza catre el insusi
void MAKESET_NOD(Node_Set* node_s, Operation* operatii)
{
	operatii->count(2);
	node_s->parinte = node_s;
	node_s->rank = 0;
}

//asociaza unui nod valoarea key dorita
Node_Set* createNode(int key)
{
	Node_Set* new_node = (Node_Set*)malloc(sizeof(Node_Set));
	if (new_node != NULL)
	{
		new_node->key = key;
	}
	return new_node;
}

//gaseste reprezentatul multimii in care se afla nodul
//reprezentatul fiecarei multimi este radacina
//euristica path compression leagă toate elementele de pe ramura cu x la rădăcina arborelui
Node_Set* FIND_SET(Node_Set* node_s, Operation* operatii)
{
	operatii->count();
	if (node_s->parinte != node_s)
	{
		operatii->count();
		node_s->parinte = FIND_SET(node_s->parinte, operatii);
	}
	return node_s->parinte;
}

//este utilizata euristica UNION BY RANK, ceea ce inseamna ca radacina cu rank-ul mai mic o sa pointeze catre radacina cu 
//rank-ul mai mare
void LINK_SET(Node_Set* node_s1, Node_Set* node_s2, Operation* operatii)
{
	operatii->count();
	if (node_s1 != node_s2)
	{
		operatii->count();
		if (node_s1->rank > node_s2->rank)
		{
			operatii->count();
			node_s2->parinte = node_s1;
		}
		else
		{
			operatii->count();
			if (node_s1->rank < node_s2->rank)
			{
				operatii->count();
				node_s1->parinte = node_s2;
			}
			else
			{
				operatii->count();
				node_s1->parinte = node_s2;
				node_s2->rank++;
			}
		}
	}
}

//combina multimile carora le apartin nodul 1 si nodul 2
void UNION(Node_Set* node_s1, Node_Set* node_s2, Operation* operatii)
{
	LINK_SET(FIND_SET(node_s1, operatii), FIND_SET(node_s2, operatii), operatii);
}

void demoDisjoinedSets()
{
	int n = 10;
	Operation operatii = pro.createOperation("Kruskal_Op", n);
	Node_Set** tree_forest = (Node_Set**)malloc(n * sizeof(Node_Set*));
	if (tree_forest != NULL)
	{
		//cream 10 multimi, fiecare formata dintr-un singur nod, cu elementele de la 1 la 10
		for (int i = 0;i < n;i++)
		{
			tree_forest[i] = MAKESET(i + 1);
			printf("%d are parintele: %d\n", tree_forest[i]->key, FIND_SET(tree_forest[i], &operatii)->key);
		}
		printf("\n");
		//facem astfel incat fiecare 2 noduri sa formeze cate o padure de arbori, sa aiba aceeasi radacina
		for (int i = 0;i < n - 1;i += 2)
			UNION(tree_forest[i + 1], tree_forest[i], &operatii);
		for (int i = 0;i < n;i++)
			printf("%d are parintele: %d\n", tree_forest[i]->key, FIND_SET(tree_forest[i], &operatii)->key);
		printf("\n");

		UNION(tree_forest[3], tree_forest[0], &operatii);

		for (int i = 0;i < n;i++)
			printf("%d are parintele: %d\n", tree_forest[i]->key, FIND_SET(tree_forest[i], &operatii)->key);
		printf("\n");

		UNION(tree_forest[4], tree_forest[9], &operatii);

		for (int i = 0;i < n;i++)
			printf("%d are parintele: %d\n", tree_forest[i]->key, FIND_SET(tree_forest[i], &operatii)->key);
		printf("\n");
	}
}

//LOMUTO's PARTITTION pentru muchii, pivotul reprezinta ultima muchie din multimea de muchii a grafului
//elementele de dinainte de pivot vor avea costul muchiilor mai mic decat cel al pivotului, iar cele de dupa costul mai mare
//returneaza pozitia unde plaseaza pivotul, pozitia de dupa cea unde se afla ultima muchie cu costul mai mic decal cel mai pivotului
int PARTITION(Muchie** E, int p, int r, Operation* operatii)
{
	operatii->count();
	Muchie* pivot = E[r];
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		operatii->count();
		if (E[j]->cost <= pivot->cost)
		{
			i++;
			operatii->count(3);
			Muchie* aux1 = E[i];
			E[i] = E[j];
			E[j] = aux1;

		}
	}
	operatii->count(3);
	Muchie* aux2 = E[i + 1];
	E[i + 1] = E[r];
	E[r] = aux2;

	return i + 1;
}

//ordoneaza muchiile in ordine crescatoare in functie de costul fiecareia
void insertionSort(Muchie** E, int p, int nr_muchii, Operation* operatii)
{
	for (int i = p + 1;i < nr_muchii + 1;i++)
	{
		operatii->count();
		Muchie* key = E[i];
		int j = i - 1;
		while (j >= p && E[j]->cost >= key->cost)
		{
			operatii->count(2);
			E[j + 1] = E[j];
			j = j - 1;
		}
		operatii->count(2);
		E[j + 1] = key;
	}
}

Muchie* createMuchie(Node_Set* nod1, Node_Set* nod2, int cost)
{
	Muchie* muchie = (Muchie*)malloc(sizeof(Muchie));
	if (muchie != NULL)
	{
		muchie->nod1 = nod1;
		muchie->nod2 = nod2;
		muchie->cost = cost;
	}
	return muchie;
}

// QuickSort este un algoritm eficient pentru siruri de lungimi mari, dar in cazul sirurilor de lungimi mici, InsertionSort
//este mai eficient decat QuickSort
//QuickSort foloseste stiva pentru adresele de intoarcere din apelurile recursive
void quickSortHibridizat(Muchie** E, int p, int r, Operation* operatii)
{
	while (p < r)
	{
		//daca lungimea sirului este mai mica decat 10, aplicam insertionSort si oprim recursivitatea
		if (r - p + 1 < 10)
		{
			insertionSort(E, p, r, operatii);
			break;
		}
		else
		{
			int q = PARTITION(E, p, r, operatii);
			//daca partea de dinainte de pivot este mai mica decat partea de dupa, sortam partea stanga a sirului
			//si ne mutam pe partea dreapta apoi
			if (q - p < r - q)
			{
				quickSortHibridizat(E, p, q - 1, operatii);
				p = q + 1;
			}
			//daca partea de dupa pivot este mai mica decat partea de dinainte, sortam partea dreapta a sirului
			//si ne mutam pe partea stanga apoi
			else
			{
				quickSortHibridizat(E, q + 1, r, operatii);
				r = q - 1;
			}
		}
	}
}

void print_muchii(Muchie** muchie, int n)
{
	for (int i = 0;i < n;i++)
		printf("%d %d %d\n", muchie[i]->nod1->key, muchie[i]->nod2->key, muchie[i]->cost);
}

//agoritm greedy pentru ca la fiecare pas adauga la graf (padurea de arbori) muchia cu cel mai mic cost posibil
Muchie** AlgorithmKruskal(Graf* graf, int nr_muchii, int* nr_muchii_arbore_rezultat, int nr_noduri, int* cost_total_arbore)
{
	Operation operatii = pro.createOperation("Kruskal_Op", nr_noduri);
	*cost_total_arbore = 0;
	*nr_muchii_arbore_rezultat = 0;
	Muchie** arbore_rezultat = (Muchie**)malloc(nr_muchii * sizeof(Muchie*));
	if (arbore_rezultat != NULL)
	{
		//cream multimile de noduri din V
		for (int i = 0; i < nr_noduri; i++)
			MAKESET_NOD(graf->V[i], &operatii);

		//ordonam muchiile din graf in ordine crescatoare dupa costul fiecareia
		quickSortHibridizat(graf->E, 0, nr_muchii - 1, &operatii);
		//print_muchii(graf->E, nr_muchii);
		//exit;

		//cu i pargurgem muchiile grafului initial
		int i_graf_initial = 0;
		//graful de acoperira minima are maxim V-1 muchii, adica nr_noduri-1 asa ca atunci cand numarul de muchii al 
		//grafului rezultat ajunge la nr_noduri-1 ne oprim
		while (*nr_muchii_arbore_rezultat < nr_noduri - 1)
		{
			Muchie* muchie_adaugata = graf->E[i_graf_initial];
			i_graf_initial++;
			Node_Set* nod_u = FIND_SET(muchie_adaugata->nod1, &operatii);
			Node_Set* nod_v = FIND_SET(muchie_adaugata->nod2, &operatii);
			//verificam daca nodurile muchiei curente fac parte din acelasi arbore, iar daca nu, acestea nu vor crea
			//un ciclu in graful rezultat, ceea ce inseamna ca putem sa adaugam muchia in arbore si sa legam nodurile
			if (nod_u != nod_v)
			{
				arbore_rezultat[(*nr_muchii_arbore_rezultat)++] = muchie_adaugata;
				*cost_total_arbore += muchie_adaugata->cost;
				UNION(nod_v, nod_u, &operatii);
			}
		}
	}
	return arbore_rezultat;
}

Graf* createGraf(int nr_noduri, int nr_muchii)
{
	Graf* G = (Graf*)malloc(sizeof(Graf));
	if (G != NULL)
	{
		G->V = (Node_Set**)malloc(nr_noduri * sizeof(Node_Set*));
		G->E = (Muchie**)malloc(nr_muchii * sizeof(Muchie*));
	}
	return G;
}

void demoAlghtKruskal()
{
	int V = 9, E = 14; //numarul de noduri, numarul de muchii la grafului
	Graf* G = createGraf(V, E);
	for (int i = 0;i < 9;i++)
		G->V[i] = createNode(i);
	G->E[0] = createMuchie(G->V[0], G->V[1], 4);
	G->E[1] = createMuchie(G->V[1], G->V[2], 8);
	G->E[2] = createMuchie(G->V[2], G->V[3], 7);
	G->E[3] = createMuchie(G->V[3], G->V[4], 9);
	G->E[4] = createMuchie(G->V[4], G->V[5], 10);
	G->E[5] = createMuchie(G->V[5], G->V[6], 2);
	G->E[6] = createMuchie(G->V[6], G->V[7], 1);
	G->E[7] = createMuchie(G->V[7], G->V[8], 7);
	G->E[8] = createMuchie(G->V[8], G->V[6], 6);
	G->E[9] = createMuchie(G->V[8], G->V[2], 2);
	G->E[10] = createMuchie(G->V[7], G->V[1], 11);
	G->E[11] = createMuchie(G->V[7], G->V[0], 8);
	G->E[12] = createMuchie(G->V[2], G->V[5], 4);
	G->E[13] = createMuchie(G->V[3], G->V[5], 14);

	int nr_muchii_graf_rezultat, cost_total;
	Muchie** arbore_rezultat = AlgorithmKruskal(G, E, &nr_muchii_graf_rezultat, V, &cost_total);
	printf("Arborele de acoperire minina are costul total %d:\n", cost_total);
	for (int i = 0; i < nr_muchii_graf_rezultat; i++)
		printf("nod1 = %d, nod2 = %d, cost = %d\n", arbore_rezultat[i]->nod1->key, arbore_rezultat[i]->nod2->key, arbore_rezultat[i]->cost);
}

void grafic_Algorithm_Kruskal()
{
	for (int n = STEP_SIZE;n <= MAX_SIZE;n += STEP_SIZE)
	{
		Graf* G = createGraf(n, n * 4);
		for (int i = 0;i < n;i++)
			G->V[i] = createNode(i);
		int* cost = (int*)malloc((static_cast<unsigned long long>(n) * 4) * sizeof(int));
		if (cost != NULL)
		{
			FillRandomArray(cost, n * 4, 1, 50, false, 0);
			int indice1 = 0, indice2 = 0;
			for (int i = 0;i < n * 4;i++)
			{
				if (indice1 == indice2)
					indice1++;
				G->E[i] = createMuchie(G->V[indice1], G->V[indice2], cost[i]);
				if (i % 6 == 0 || i % 6 == 1 || i % 6 == 2)
					indice1++;
				else
					indice2++;
				if (indice2 == n)
					indice2 = 50;
				if (indice1 == n)
					indice1 = 0;
			}
			//print_muchii(G->E, n * 4);
			//break;
			int nr_muchii_arbore_rezultat, cost_total;
			Muchie** arbore_rezultat = AlgorithmKruskal(G, n * 4, &nr_muchii_arbore_rezultat, n, &cost_total);
		}
	}
	pro.showReport();
}

int main()
{
	//demoDisjoinedSets();
	//demoAlghtKruskal();
	//grafic_Algorithm_Kruskal();
	return 0;
}