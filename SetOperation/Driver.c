#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>

typedef struct set
{
	int size;
	int *elems;
}Set;
typedef struct pair
{
	int x;
	int y;
}Pair;
typedef struct cartesian
{
	int size;
	Pair **values;
}Cartesian;

int isExist(Set* set, int elem);
int getCardinality(Set *set);

void getUnion(Set* a, Set* b, Set** unionSet);
void getIntersection(Set* a, Set* b, Set** intersectionSet);
void getComplement(Set* u, Set* a, Set** complementSet);
void getDifference(Set* a, Set* b, Set** differenceSet);
void getSymmetricDifference(Set* a, Set* b, Set** symDifferenceSet);
void getCartesianProduct(Set* a, Set* b, Cartesian** cartesianSet);
void printSet(Set* set);
void printCartesian(Cartesian* set);
void freeSet(Set* set);
void freeCartesian(Cartesian* set);

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetBreakAlloc(76);
//	_CrtSetBreakAlloc(78);
	
	int u[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int a[] = { 1, 2, 3, 4, 5 };
	int b[] = { 4, 5, 6, 7, 8 };
	int c[] = { 100, 200 };

	Set setU = { (sizeof(u) / sizeof(int)), u };
	Set setA = { (sizeof(a) / sizeof(int)), a };
	Set setB = { (sizeof(b) / sizeof(int)), b };
	Set setC = { (sizeof(c) / sizeof(int)), c };

	Set* unionSet = NULL;
	Set* intersectionSet = NULL;
	Set* complementSet1 = NULL;
	Set* complementSet2 = NULL;
	Set* differenceSet1 = NULL;
	Set* differenceSet2 = NULL;
	Set* symDifferenceSet = NULL;
	Cartesian* cartesianSet = NULL;

	getUnion(&setA, &setB, &unionSet);
	getIntersection(&setA, &setB, &intersectionSet);
	getComplement(&setU, &setA, &complementSet1);
	getComplement(&setU, &setB, &complementSet2);
	getDifference(&setA, &setB, &differenceSet1);
	getDifference(&setB, &setA, &differenceSet2);
	getSymmetricDifference(&setA, &setB, &symDifferenceSet);
	getCartesianProduct(&setA, &setC, &cartesianSet);

	printf(" << Set Operations >>\n");
	printf(" 1. Union A and B\n");
	printf(" = ");
	printSet(unionSet);
	printf(" 2. Intersection A and B\n");
	printf(" = ");
	printSet(intersectionSet);
	printf(" 3. Complement A\n");
	printf(" = ");
	printSet(complementSet1);
	printf(" 4. Complement B\n");
	printf(" = ");
	printSet(complementSet2);
	printf(" 5. Difference A and B\n");
	printf(" = ");
	printSet(differenceSet1);
	printf(" 6. Difference B and A\n");
	printf(" = ");
	printSet(differenceSet2);
	printf(" 7. SymmetricDifference A and B\n");
	printf(" = ");
	printSet(symDifferenceSet);
	printf(" 8.Cartesian Product A and C\n");
	printf(" = ");
	printCartesian(cartesianSet);
	freeSet(unionSet);
	freeSet(intersectionSet);
	freeSet(complementSet1);
	freeSet(complementSet2);
	freeSet(differenceSet1);
	freeSet(differenceSet2);
	freeSet(symDifferenceSet);
	freeCartesian(cartesianSet);
	_CrtDumpMemoryLeaks();
	return 0;
}

int getCardinality(Set *set)
{
	return set->size;
}

int isExist(Set* set, int elem)
{
	int i;
	for (i = 0; i < set->size; i++)
	{
		if (set->elems[i] == elem)
			return 1;
	}
	return 0;
}


void getUnion(Set* a, Set* b, Set** unionSet)
{
	int *aBegin = NULL;
	int *aEnd = NULL;
	int *bBegin = NULL;
	int *bEnd = NULL;

	*unionSet = (Set*)malloc(sizeof(Set));
	(*unionSet)->size = 0;
	(*unionSet)->elems = (int*)malloc(sizeof(int) * (a->size + b->size));

	aBegin = a->elems;
	aEnd = a->elems + a->size;
	bBegin = b->elems;
	bEnd = b->elems + b->size;

	while (aBegin != aEnd && bBegin != bEnd)
	{
		if (*aBegin < *bBegin)
		{
			(*unionSet)->elems[(*unionSet)->size++] = *aBegin;
			aBegin++;
		}
		else if (*aBegin > *bBegin)
		{
			(*unionSet)->elems[(*unionSet)->size++] = *bBegin;
			bBegin++;
		}
		else
		{
			(*unionSet)->elems[(*unionSet)->size++] = *bBegin;
			bBegin++;
			aBegin++;
		}
	}

	while (aBegin != aEnd)
	{
		(*unionSet)->elems[(*unionSet)->size++] = *aBegin;
		aBegin++;
	}

	while (bBegin != bEnd)
	{
		(*unionSet)->elems[(*unionSet)->size++] = *bBegin;
		bBegin++;
	}
}

void getIntersection(Set* a, Set* b, Set** intersectionSet)
{
	int i = 0;
	int j = 0;

	*intersectionSet = (Set*)malloc(sizeof(Set));
	(*intersectionSet)->size = 0;
	(*intersectionSet)->elems = (int*)malloc(sizeof(int) * (a->size + b->size));

	for (i = 0; i < a->size; i++)
	{
		for (j = 0; j < b->size; j++)
		{
			if (a->elems[i] == b->elems[j])
			{
				(*intersectionSet)->elems[(*intersectionSet)->size++] = a->elems[i];
				break;
			}
		}
	}
}

void getComplement(Set* u, Set* a, Set** complementSet)
{
	int i = 0;
	int j = 0;
	int judge = 0;

	*complementSet = (Set*)malloc(sizeof(Set));
	(*complementSet)->size = 0;
	(*complementSet)->elems = (int*)malloc(sizeof(int) * u->size);

	for (i = 0; i < u->size; i++)
	{
		judge = 0;
		for (j = 0; j < a->size; j++)
		{
			if (u->elems[i] == a->elems[j])
			{
				judge = 1;
				break;
			}
		}
		if (!judge)
		{
			(*complementSet)->elems[(*complementSet)->size++] = u->elems[i];
		}
	}
}

void getDifference(Set* a, Set* b, Set** differenceSet)
{
	int i = 0;
	int j = 0;
	int judge = 0;

	*differenceSet = (Set*)malloc(sizeof(Set));
	(*differenceSet)->size = 0;
	(*differenceSet)->elems = (int*)malloc(sizeof(int) * (a->size + b->size));

	for (i = 0; i < a->size; i++)
	{
		judge = 0;
		for (j = 0; j < b->size; j++)
		{
			if (a->elems[i] == b->elems[j])
			{
				judge = 1;
				break;
			}
		}
		if (!judge)
		{
			(*differenceSet)->elems[(*differenceSet)->size++] = a->elems[i];
		}
	}
}

void getSymmetricDifference(Set* a, Set* b, Set** symDifferenceSet)
{
	int i = 0;
	int j = 0;
	int judge = 0;

	Set *intersectionSet = NULL;
	Set *unionSet = NULL;

	getUnion(a, b, &unionSet);
	getIntersection(a, b, &intersectionSet);
	*symDifferenceSet = (Set*)malloc(sizeof(Set));
	(*symDifferenceSet)->size = 0;
	(*symDifferenceSet)->elems = (int*)malloc(sizeof(int) * unionSet->size);

	for (i = 0; i < unionSet->size; i++)
	{
		judge = 0;
		for (j = 0; j < intersectionSet->size; j++)
		{
			if (unionSet->elems[i] == intersectionSet->elems[j])
			{
				judge = 1;
				break;
			}
		}
		if (!judge)
		{
			(*symDifferenceSet)->elems[(*symDifferenceSet)->size++] = unionSet->elems[i];
		}
	}

	freeSet(intersectionSet);
	intersectionSet = NULL;
	freeSet(unionSet);
	unionSet = NULL;
}

static void getCartesianProduct(Set *a, Set *b, Cartesian **cartesianSet) {
	*cartesianSet = (Cartesian*)malloc(sizeof(Cartesian));
	(*cartesianSet)->size = 0;
	(*cartesianSet)->values = (Pair**)malloc(sizeof(Pair*) * (a->size * b->size));

	int i, j, bgn = 0;
	for (i = 0; i < getCardinality(a); i++) {
		for (j = 0; j < getCardinality(b); j++) {
			(*cartesianSet)->values[bgn] = (Pair*)malloc(sizeof(Pair));
			(*cartesianSet)->values[bgn]->x = a->elems[i];
			(*cartesianSet)->values[bgn]->y = b->elems[j];
			(*cartesianSet)->size++;
			bgn++;
		}
	}
}

void printSet(Set* set)
{
	int i = 0;
	printf(" {");
	for (i = 0; i < set->size; i++)
	{
		if (i < set->size - 1)
			printf(" %d,", set->elems[i]);
		else
			printf(" %d ", set->elems[i]);
	}
	printf(" }\n");
}

static void printCartesian(Cartesian *set) {
	printf("{ ");
	for (int i = 0; i < set->size; i++) {
		if (i == set->size - 1) 
		{
			printf("<%d, %d>", set->values[i]->x, set->values[i]->y);
		}
		else 
		{
			printf("<%d, %d >, ", set->values[i]->x, set->values[i]->y);
		}
	}
	printf("}\n");
}
void freeSet(Set* set)
{
	free(set->elems);
	set->elems = NULL;
	free(set);
	set = NULL;
}

void freeCartesian(Cartesian* set)
{
	int i;
	for (i = 0; i < set->size; i++)
	{
		free(set->values[i]);
		set->values[i] = NULL;
	}
	free(set->values);
	set->values = NULL;
	free(set);
	set = NULL;
}