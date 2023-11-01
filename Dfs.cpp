#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include "Profiler.h"
using namespace std;

Profiler pro("DFS");

typedef struct nodetype
{
	int key;
	struct nodetype* next;
}Node;

typedef struct _Lista
{
	Node* first;
	Node* last;
}Lista;

typedef struct _GRAF_INFO
{
	int nr_noduri;
	Lista* lista_adiacenta; 
	int* pi; //vectorul de parinti
	int* color;
	int* d; //timpul de descoperire
	int* f; //timpul de finalizare
}Graf;

enum { WHITE, GRAY, BLACK };

Graf* createGraf(int nr_noduri)
{
	Graf* graf = (Graf*)malloc(sizeof(Graf));
	Lista* lista = (Lista*)malloc(nr_noduri * sizeof(Lista));
	graf->color = (int*)malloc(nr_noduri * sizeof(int));
	graf->pi = (int*)malloc(nr_noduri * sizeof(int));
	graf->d = (int*)malloc(nr_noduri * sizeof(int));
	graf->f = (int*)malloc(nr_noduri * sizeof(int));
	graf->nr_noduri = nr_noduri;
	for (int i = 0; i < nr_noduri; i++)
	{
		lista[i].first = NULL;
		lista[i].last = NULL;
	}
	graf->lista_adiacenta = lista;
	return graf;
}

Node* create_node(int key)
{
	Node* node = (Node*)malloc(sizeof(Node));
	if (node != NULL)
	{
		node->key = key;
		node->next = NULL;
	}
	return node;
}

void insert_last(Lista* lista, int key)
{
	Node* node = create_node(key);
	if (lista->first == NULL)
		lista->first = lista->last = node;
	else
	{
		lista->last->next = node;
		lista->last = node;
	}
}

void insert_first(Lista* lista, int key)
{
	Node* node = create_node(key);
	if (lista->first == NULL)
	{
		lista->first = lista->last = node;
	}
	else
	{
		node->next = lista->first;
		lista->first = node;
	}
}

void add_muchie(Graf* graf, int sursa, int destinatie)
{
	insert_last(&(graf->lista_adiacenta[sursa]), destinatie);
}

void print_lista(Lista* lista)
{
	Node* node = lista->first;
	while (node != NULL)
	{
		printf("%d ", node->key);
		node = node->next;
	}
}

void print_graf(Graf* graf)
{
	printf("Graful reprezentat prin liste de adiacenta:\n");
	for (int i = 0; i < graf->nr_noduri; i++)
	{
		printf("%d: ", i);
		print_lista(&(graf->lista_adiacenta[i]));
		printf("\n");
	}
}

void DFS_visit(Graf* graf, int nod_sursa, int* time, bool** cycle, Operation* operatii, Lista* lista)
{
	*time = *time + 1;
	operatii->count(2);
	graf->d[nod_sursa] = *time;
	graf->color[nod_sursa] = GRAY;
	Node* p = graf->lista_adiacenta[nod_sursa].first;
	while (p != NULL)
	{
		int w = p->key;
		//operatii->count();
		if (graf->color[w] == GRAY)
		{
			//operatii->count();
			(*cycle)[nod_sursa] = true;
		}
		operatii->count();
		if (graf->color[w] == WHITE)
		{
			operatii->count();
			graf->pi[w] = nod_sursa;
			DFS_visit(graf, w, time, cycle, operatii, lista);
		}
		p = p->next;
	}
	insert_first(lista, nod_sursa);
	operatii->count(2);
	graf->color[nod_sursa] = BLACK;
	*time = *time + 1;
	graf->f[nod_sursa] = *time;
}

//se trece o data printr-o muchie
//O(nr_noduri + nr_muchii)
void DFS(Graf* graf, bool** cycle, Operation* operatii, Lista* lista)
{
	for (int i = 0;i < graf->nr_noduri;i++)
	{
		operatii->count(2);
		graf->color[i] = WHITE;
		graf->pi[i] = -1;
	}
	int time = 0;
	for (int i = 0;i < graf->nr_noduri;i++)
	{
		operatii->count();
		if(graf->color[i] == WHITE)
			DFS_visit(graf, i, &time, cycle, operatii, lista);
	}
}

//nu se poate face sortare topologica pe un graf care contine cicluri
Lista sortareTopologica(Graf* graf)
{
	//facem DFS pe graf, iar in functie de timpul de finalizarea al fiecarui nod, le adaugam in lista in ordine descrescatoare
	Lista lista = { NULL, NULL };
	Operation op = pro.createOperation("DFS", graf->nr_noduri);
	int* array_aux = (int*)malloc(graf->nr_noduri * sizeof(int));
	bool graf_ciclic = false;
	bool* cycle = (bool*)malloc(graf->nr_noduri * sizeof(bool));
	for (int i = 0;i < graf->nr_noduri;i++)
		cycle[i] = false;
	DFS(graf, &cycle, &op, &lista);
	for (int i = 0;i < graf->nr_noduri;i++)
	{
		if (cycle[i] == true)
		{
			graf_ciclic = true;
			break;
		}
	}
	if (graf_ciclic == true)
	{
		printf("Nu se poate face sortare topologica pe acest graf. Contine cicluri.");
		exit(2);
	}
	else
	{
		return lista;
	}
}

void pretty_print_tree(int* parent_array, int n, int parent, int level)
{
	for (int i = 0;i < n;i++)
	{
		if (parent_array[i] == parent)
		{
			for (int j = 0;j < level;j++)
				printf("\t");
			printf("%d\n", i);
			pretty_print_tree(parent_array, n, i, level + 1);
		}
	}
}

void demo_DFS()
{
	Graf* graf = createGraf(6);
	Operation op = pro.createOperation("DFS", graf->nr_noduri);

	add_muchie(graf, 0, 1);
	add_muchie(graf, 0, 3);
	add_muchie(graf, 1, 2);
	add_muchie(graf, 2, 3);
	add_muchie(graf, 2, 4);
	add_muchie(graf, 3, 1);
	add_muchie(graf, 4, 5);
	
	print_graf(graf);

	bool* cycle = (bool*)malloc(graf->nr_noduri * sizeof(bool));
	Lista lista = { NULL, NULL };
	DFS(graf, &cycle, &op, &lista);

	printf("Vectorul de parinti: ");
	for (int i = 0; i < graf->nr_noduri; i++)
		printf("%d ", graf->pi[i]);
	printf("\nTimpii de descoperire: ");
	for (int i = 0; i < graf->nr_noduri; i++)
		printf("%d ", graf->d[i]);
	printf("\nTimpii de finalizare: ");
	for (int i = 0; i < graf->nr_noduri; i++)
		printf("%d ", graf->f[i]);
	printf("\n");
	pretty_print_tree(graf->pi, graf->nr_noduri, -1, 0);
}

void demo_SORTARE_TOPOLOGICA()
{
	Graf* graf = createGraf(6);

	add_muchie(graf, 0, 5);
	add_muchie(graf, 1, 0);
	add_muchie(graf, 3, 1);
	add_muchie(graf, 3, 2);
	add_muchie(graf, 4, 2);
	add_muchie(graf, 4, 5);
	//add_muchie(graf, 5, 1);

	print_graf(graf);

	Lista lista_sortare_topologica = sortareTopologica(graf);
	print_lista(&lista_sortare_topologica);
}

int minim(int a, int b)
{
	return (a < b) ? a : b;
}

int nrcomponents;
int indice = 0;

//SCC - Strongly Connected Components
void STRONG_CONNECT(Graf* graf, int nod, int* index_T, stack<int>& stiva_T, int* lowLink_T, int* comp_T, bool* onStack_T)
{
	index_T[nod] = indice;
	lowLink_T[nod] = indice;
	indice++;
	stiva_T.push(nod);
	onStack_T[nod] = true;
	Node* p = graf->lista_adiacenta[nod].first;
	while (p != NULL)
	{
		int w = p->key;
		if (index_T[w] == -1)
		{  
			STRONG_CONNECT(graf, w, index_T, stiva_T, lowLink_T, comp_T, onStack_T);
			lowLink_T[nod] = minim(lowLink_T[nod], lowLink_T[w]);
		}
		else if (onStack_T[w] == true)  
			lowLink_T[nod] = minim(lowLink_T[nod], index_T[w]);
		p = p->next;
	}
	int top = 0;
	if (lowLink_T[nod] == index_T[nod])
	{
		nrcomponents++;
		while (stiva_T.top() != nod)
		{
			top = stiva_T.top();
			comp_T[top] = nrcomponents;
			printf("%d ", top);
			stiva_T.pop();
			onStack_T[top] = false;
		}
		top = stiva_T.top();
		printf("%d\n", top);
		onStack_T[top] = false;
		stiva_T.pop();
	}
}

void TARJAN_SCC(Graf* graf)
{
	int* index_T = (int*)malloc(graf->nr_noduri * sizeof(int));
	stack<int> stiva_T;
	int* lowLink_T = (int*)malloc(graf->nr_noduri * sizeof(int));
	int* comp_T = (int*)malloc(graf->nr_noduri * sizeof(int));
	bool* onStack_T = (bool*)malloc(graf->nr_noduri * sizeof(bool));
	if (index_T != NULL && lowLink_T != NULL && comp_T != NULL && onStack_T != NULL)
	{
		for (int i = 0; i < graf->nr_noduri; i++)
		{
			index_T[i] = -1;
			lowLink_T[i] = -1;
			onStack_T[i] = false;
			comp_T[i] = 0;
		}
		for (int i = 0; i < graf->nr_noduri; i++)
		{
			if (index_T[i] == -1)
				STRONG_CONNECT(graf, i, index_T, stiva_T, lowLink_T, comp_T, onStack_T);
		}
	}
}

void demo_TARJAN()
{
	Graf* graf = createGraf(10);
	add_muchie(graf, 0, 1);
	add_muchie(graf, 1, 2);
	add_muchie(graf, 2, 0);
	add_muchie(graf, 2, 3);
	add_muchie(graf, 3, 0);
	add_muchie(graf, 2, 4);
	add_muchie(graf, 4, 5);
	add_muchie(graf, 5, 6);
	add_muchie(graf, 6, 4);
	add_muchie(graf, 6, 2);
	add_muchie(graf, 5, 7);
	add_muchie(graf, 7, 8);
	add_muchie(graf, 8, 9);
	add_muchie(graf, 8, 5);
	add_muchie(graf, 9, 5);

	print_graf(graf);
	printf("\n");

	TARJAN_SCC(graf);
	printf("Numarul de componente tare conexe este: %d", nrcomponents);
}

void grafic_DFS_nrNoduriFix()
{
	Lista lista = { NULL, NULL };
	Graf* graf = createGraf(100);
	bool* cycle = (bool*)malloc(graf->nr_noduri * sizeof(bool));
	for (int nr_muchii = 1000;nr_muchii <= 4500;nr_muchii += 100)
	{
		Operation operatii = pro.createOperation("DFS", nr_muchii);
		int k = 0;
		int* array = (int*)malloc(2 * sizeof(int));
		while (k < nr_muchii)
		{
			FillRandomArray(array, 2, 0, 99, true);
			bool ok = false;
			Node* p = graf->lista_adiacenta[array[0]].first;
			while (p != NULL)
			{
				int w = p->key;
				//daca sunt legate, sar peste si nu ii mai leg
				if (w == array[1]);
				{
					ok = true;
					break;
				}
				if (ok) continue;
				p = p->next;

			}
			add_muchie(graf, array[0], array[1]);
			k++;
		}
		DFS(graf, &cycle, &operatii, &lista);
	}
	pro.showReport();
}

void grafic_DFS_nrMuchiiFix()
{
	Lista lista = { NULL, NULL };
	for (int nr_noduri = 100;nr_noduri <= 200;nr_noduri += 10)
	{
		Graf* graf = createGraf(nr_noduri);
		bool* cycle = (bool*)malloc(graf->nr_noduri * sizeof(bool));
		Operation operatii = pro.createOperation("DFS", nr_noduri);
		int k = 0;
		int* array = (int*)malloc(2 * sizeof(int));
		while (k < 4500)
		{
			FillRandomArray(array, 2, 0, nr_noduri - 1, true);
			bool ok = false;
			Node* p = graf->lista_adiacenta[array[0]].first;
			while (p != NULL)
			{
				int w = p->key;
				//daca sunt legate, sar peste si nu ii mai leg
				if (w == array[1]);
				{
					ok = true;
					break;
				}
				if (ok) continue;
				p = p->next;

			}
			add_muchie(graf, array[0], array[1]);
			k++;
		}
		//print_graf(graf);
		//break;
		DFS(graf, &cycle, &operatii, &lista);
	}
	pro.showReport();
}

int main()
{
	//demo_DFS();
	//demo_SORTARE_TOPOLOGICA();
	demo_TARJAN();
	//grafic_DFS_nrNoduriFix();
	//grafic_DFS_nrMuchiiFix();
	return 0;
}