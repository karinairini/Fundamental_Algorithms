#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler pro("Tabele de dispersie");

typedef struct 
{
	int id;
	char name[30];
}Entry;

Entry createEntry(int id, char* name)
{
	Entry entry ;
	entry.id = id;
	strcpy(entry.name, name);

	return entry;
}

void createHashTable(Entry* hash_table, int size_table)
{
	//initializez toate id-urile cu -1 ceea ce inseamna ca locatiile sunt libere si name cu NULL
	for (int i = 0;i < size_table;i++)
	{
		hash_table[i].id = -1;
		strcpy(hash_table[i].name, "NULL");
	}
}

void print_array(int array[], int n)
{
	for (int i = 0;i < n;i++)
	{
		printf("%d ", array[i]);
	}
}

void print_table(Entry* hash_table, int size_table)
{
	for (int i = 0;i < size_table;i++)
		printf("%d %s\n", hash_table[i].id, hash_table[i].name);
	printf("\n");
}

int quadratic_probing(int k, int size_table, int i)
{
	return (k % size_table + 2 * i + 3 * i * i) % size_table;
}

void hash_insert(Entry* hash_table, Entry entry, int size_table)
{
	int i = 0;
	while (i <= size_table)
	{
		int j = quadratic_probing(entry.id, size_table, i);
		if (hash_table[j].id == -1 || hash_table[j].id == -3)
		{
			hash_table[j].id = entry.id;
			strcpy(hash_table[j].name, entry.name);
			break;
		}
		else
			i++;
	}
}

int hash_search(Entry* hash_table, Entry entry, int size_table)
{
	int i = 0;
	int j = quadratic_probing(entry.id, size_table, i);
	while (hash_table[j].id != -1 && i < size_table)
	{
		if (strcmp(hash_table[j].name, entry.name) == 0)
			return j;
		i++;
		j = quadratic_probing(entry.id, size_table, i);
	} 
	return -1;
}

void hash_delete(Entry* hash_table, Entry entry, int size_table)
{
	int pozitie = hash_search(hash_table, entry, size_table);
	if (pozitie != -1)
	{
		hash_table[pozitie].id = -3;
		strcpy(hash_table[pozitie].name, "NULL");
	}
}

void demo_inserare()
{
	//N reprezinta nr de elemente pe care le poate contine tabela
	int N = 11;
	Entry* hash_table = (Entry*)malloc(N * sizeof(Entry));
	if (hash_table != NULL)
	{
		createHashTable(hash_table, N);

		int factorDeUmplere = 95;

		//M reprezinta nr de elemente pe care le va contine tabela - 10
		int M = ((double)factorDeUmplere)/100 * N;
		
		//generez un vector cu elemente random pentru campul id
		int* array = (int*)malloc(M * sizeof(int));
		if (array != NULL)
		{
			FillRandomArray(array, M, 1, 100, true, 0);
			print_array(array, M);
			printf("\n");

			char names[10][30] = { "Ioana", "Ana", "Dana", "Laur", "Alin", "Mara", "Karina", "Antonia", "Raul", "Lilian" };

			for (int i = 0;i < M;i++)
			{
				Entry entry = createEntry(array[i], names[i]);
				hash_insert(hash_table, entry, N);
			}
			print_table(hash_table, N);
		}
	}
}

void demo_cautare_stergere()
{
	//N reprezinta nr de elemente pe care le poate contine tabela
	int N = 11;
	Entry* hash_table = (Entry*)malloc(N * sizeof(Entry));
	if (hash_table != NULL)
	{
		createHashTable(hash_table, N);

		//generez un vector cu elemente random pentru campul id
		int* array = (int*)malloc(N * sizeof(int));
		if (array != NULL)
		{
			FillRandomArray(array, N, 1, 100, true, 0);
			print_array(array, N);
			printf("\n");

			char names[12][30] = { "Ioana", "Ana", "Dana", "Laur", "Alin", "Mara", "Karina", "Antonia", "Raul", "Lilian", "Rodica" };

			for (int i = 0;i < N;i++)
			{
				Entry entry = createEntry(array[i], names[i]);
				hash_insert(hash_table, entry, N);
			}
			print_table(hash_table, N);

			//caut id-ul intrarii cu numele Karina
			char name_search[30] = "Karina";
			int id_search = 0;
			for (int i = 0;i < N;i++)
			{
				if (strcmp(hash_table[i].name, name_search) == 0)
					id_search = hash_table[i].id;
			}

			Entry entry = createEntry(id_search, name_search);
			int pozitie = hash_search(hash_table, entry, N);
			printf("Am gasit intrarea cu id-ul %d si numele %s la pozitia %d.\n", id_search, name_search, pozitie);
			printf("\n");

			char name_search_2[30] = "Raul";
			int id_search_2 = 0;
			for (int i = 0;i < N;i++)
			{
				if (strcmp(hash_table[i].name, name_search_2) == 0)
					id_search_2 = hash_table[i].id;
			}

			entry = createEntry(id_search_2, name_search_2);
			pozitie = hash_search(hash_table, entry, N);
			hash_delete(hash_table, entry, N);
			printf("Am sters intrarea cu id-ul %d si numele %s la pozitia %d.\n", id_search_2, name_search_2, pozitie);
			print_table(hash_table, N);
		}

	}
}

int main()
{
	//demo_inserare();
	demo_cautare_stergere();
	return 0;
}