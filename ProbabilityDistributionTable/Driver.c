#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#ifndef DATATYPE
#define DATATYPE
typedef int EType;
typedef float PType;
#endif
#ifndef PAIR
#define PAIR
typedef struct {
	EType num;
	PType prob;
} Pair;
#endif
#ifndef PTABLE
#define PTABLE
typedef struct {
	int size;
	float mean;
	float var;
	Pair **elem;
} PTable;
#endif

static PTable *MakePTable(const int size);
static PTable *MakeProbabilityDistributionTable(const EType *v1, const PType *v2, const int length);
static Pair *MakePair(const EType v1, const PType v2);
static void ComputeMean(PTable *ptable);
static void ComputeVariance(PTable *ptable);
static void PrintProbabilityDistributionTable(PTable *ptable);
static void PrintPair(Pair *elem);
static void DestroyPTable(PTable *ptable);
static void DestroyPair(Pair **elem, int size);

int main(int argc, char *argv[])
{
	int v1[] = { 0, 1, 2, 3, 4, 5 };
	float v2[] = { 0.05f, 0.1f, 0.2f, 0.4f, 0.15f, 0.1f };
	PTable *table = NULL;
	table = MakeProbabilityDistributionTable(v1, v2, (sizeof(v1) / sizeof(int)));
	PrintProbabilityDistributionTable(table);
	ComputeMean(table);
	printf("Mean = %6.3f\n", table->mean);
	ComputeVariance(table);
	printf("Variance = %6.3f\n", table->var);
	DestroyPTable(table);
	return 0;
}

static PTable *MakePTable(const int size)
{

}

static PTable *MakeProbabilityDistributionTable(const EType *v1, const PType *v2, const int length)
{
	int i;
	PTable *tempTable = NULL;
	tempTable = (PTable*)malloc(sizeof(PTable));
	tempTable->size = length;
	tempTable->elem = (Pair**)malloc(sizeof(Pair*) * length);

	for (i = 0; i < tempTable->size; ++i)
	{
		tempTable->elem[i] = MakePair(v1[i], v2[i]);
	}

	return tempTable;
}

static Pair *MakePair(const EType v1, const PType v2)
{
	Pair *tempPair = NULL;
	tempPair = (Pair*)malloc(sizeof(Pair));
	tempPair->num = v1;
	tempPair->prob = v2;

	return tempPair;
}

static void ComputeMean(PTable *pTable)
{
	int i;
	float mean = 0;
	for (i = 0; i < pTable->size; ++i)
	{
		mean += (pTable->elem[i]->num * pTable->elem[i]->prob);
	}

	pTable->mean = mean;
}

static void ComputeVariance(PTable *pTable)
{
	int i;
	float variance = 0;

	for (i = 0; i < pTable->size; ++i)
	{
		variance += (((pTable->elem[i]->num - pTable->mean) * (pTable->elem[i]->num - pTable->mean)) * pTable->elem[i]->prob);
	}

	pTable->var = variance;
}

static void PrintProbabilityDistributionTable(PTable *pTable)
{
	int i;
	printf("\t Probability Distrivution Table \n");
	printf("---------------------------------------------------\n");
	PrintPair(pTable->elem, 6);
}

static void PrintPair(Pair **elem, int size)
{
	int i;
	printf(" x : ");
	for (i = 0; i < size; ++i)
	{
		printf("%5d", elem[i]->num);
	}

	printf("\np(x) : ");
	for (i = 0; i < size; ++i)
	{
		printf("%5.2f", elem[i]->prob);
	}
	printf("\n");
}

static void DestroyPTable(PTable *ptable)
{

}

static void DestroyPair(Pair *elem)
{

}