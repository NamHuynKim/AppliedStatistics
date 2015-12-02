#define _CRT_SECURE_NO_WARNINGS

#include "Header.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

static DataSet *CreateDataSet(const int rowsDataSet, const int colsDataSet, const int lengthDataSet);
static void DestroyDataSet(DataSet *dataSet);

static StemNode *CreateStemNode(const int elemSize, const int stemValue);
static void DestroyStemNode(StemNode *stemNode);

static StemPlotList *CreateStemPlotList(void);
static void DestroyStemPlotList(StemPlotList *stemPlotList);

static void ComputeStemPlotList(StemPlotList *stemPlotList, DataSet * dataSet);

static void AddStemNode(StemPlotList *stemPlotList, StemNode *stemNode);
static void PrintLeaf(StemNode *stemNode);
static void PrintStemPlotList(StemPlotList *stemPlotList);
static void PrintData(DataSet *dataSet);
static void SortQuick(EType *dataSet, int posLeft, int posRight);


int main(void)
{
	StemPlotList* stemPlotList;
	DataSet *dataSet;

	dataSet = CreateDataSet(5, 10, 50);
	printf("Print DataSet \n");
	PrintData(dataSet);

	stemPlotList = CreateStemPlotList();
	ComputeStemPlotList(stemPlotList, dataSet);
	PrintStemPlotList(stemPlotList);

	DestroyDataSet(dataSet);
	DestroyStemPlotList(stemPlotList);
	system("pause");
	return 0;
}

static DataSet *CreateDataSet(const int rowsDataSet, const int colsDataSet, const int lengthDataSet)
{
	FILE *infile;
	DataSet *dataSet = NULL;
	EType *bgn = NULL, *end = NULL;

	infile = fopen("Sample.dat", "r");
	dataSet = (DataSet*)malloc(sizeof(DataSet));
	fscanf(infile, "%d %d %d", &dataSet->rows, &dataSet->cols, &dataSet->length);

	dataSet->elem = (EType*)malloc(sizeof(EType*)*dataSet->length);
	memset(dataSet->elem, 0x00, (dataSet->length * sizeof(EType)));

	bgn = dataSet->elem;
	end = dataSet->elem + dataSet->length;

	while (fscanf(infile, "%d", bgn++) != EOF)
	{
		if (bgn == end)
			break;
	}

	fclose(infile);
	return dataSet;
}

static void DestroyDataSet(DataSet *dataSet)
{
	if (!dataSet)
		return;
	free(dataSet->elem);
	dataSet->elem = NULL;
	free(dataSet);
	dataSet = NULL;
}

static StemNode *CreateStemNode(const int elemSize, const int stemValue)
{
	StemNode *stemNode = NULL;
	stemNode = (StemNode*)malloc(sizeof(StemNode));

	stemNode->stem = stemValue;
	stemNode->freq = elemSize;
	stemNode->next = NULL;
	stemNode->leaf = (EType*)calloc(elemSize, sizeof(EType) * elemSize);

	return stemNode;
}

static void DestroyStemNode(StemNode *stemNode)
{
	free(stemNode->leaf);
	stemNode->leaf = NULL;
	free(stemNode);
	stemNode = NULL;
}

static StemPlotList *CreateStemPlotList(void)
{
	StemPlotList *list = NULL;
	list = (StemPlotList*)malloc(sizeof(StemPlotList));
	list->head = NULL;
	list->tail = NULL;

	return list;
}

static void DestroyStemPlotList(StemPlotList *stemPlotList)
{
	StemNode *deleteNode = NULL, *tempNode = NULL;
	tempNode = stemPlotList->head;
	while (tempNode != NULL)
	{
		deleteNode = tempNode;
		tempNode = tempNode->next;
		DestroyStemNode(deleteNode);
	}
	free(stemPlotList);
	stemPlotList = NULL;
}

static  void ComputeStemPlotList(StemPlotList *stemPlotList, DataSet * dataSet)
{
	EType *bgn = NULL, *end = NULL;
	EType *tempDataStore;
	StemNode *tempNode = NULL;
	int stemValue, elemSize;
	int i, j;

	tempDataStore = (EType *)calloc(dataSet->length, sizeof(EType) * dataSet->length);
	SortQuick(dataSet->elem, 0, dataSet->length - 1);
	printf("\n");

	bgn = dataSet->elem;
	end = dataSet->elem + dataSet->length;

	stemValue = 0;
	elemSize = 0;

	while (1)
	{
		if (stemValue != *bgn / 10 && elemSize != 0) {
			tempNode = CreateStemNode(elemSize, stemValue);
			for (i = 0; i < elemSize; i++)
			{
				tempNode->leaf[i] = tempDataStore[i];
				tempDataStore[i] = 0;
			}
			AddStemNode(stemPlotList, tempNode);

			stemValue = *bgn / 10;
			elemSize = 0;
			tempDataStore[elemSize++] = *bgn % 10;
			if (bgn == end)
				break;
		}
		else {
			tempDataStore[elemSize++] = *bgn % 10;
			stemValue = *bgn / 10;
		}
		bgn++;
	}
}

static void AddStemNode(StemPlotList *stemPlotList, StemNode *stemNode)
{
	StemNode *lastStemNode = NULL;

	if (stemPlotList->head == NULL) {
		stemPlotList->head = stemNode;
		stemPlotList->tail = stemNode;
	}
	else if (stemPlotList->head == stemPlotList->tail) {
		stemPlotList->head->next = stemNode;
		stemPlotList->tail = stemNode;
	}
	else {
		lastStemNode = stemPlotList->head;
		while (lastStemNode->next != NULL)
		{
			lastStemNode = lastStemNode->next;
		}
		lastStemNode->next = stemNode;
		stemPlotList->tail = stemNode;
	}
}

static void PrintLeaf(StemNode *stemNode)
{
	EType *bgn = NULL, *end = NULL;
	bgn = stemNode->leaf;
	end = stemNode->leaf + stemNode->freq;
	printf("\t%d ( %2d) : ", stemNode->stem, stemNode->freq);
	while (bgn != end)
	{
		printf(PRNFORMAT, *bgn++);
	}
	printf("\n");
}

static void PrintStemPlotList(StemPlotList *stemPlotList)
{
	StemNode* bgn = stemPlotList->head;

	printf("Stem Plot List \n\n");
	printf("   Stem (Freq'):           Leaf\n");
	printf("-----------------------------------------------\n");
	while (bgn != NULL)
	{
		PrintLeaf(bgn);
		bgn = bgn->next;
	}
}

static void PrintData(DataSet *dataSet)
{
	DataSet *setData = NULL;
	EType *begin = NULL;
	EType *end = NULL;

	int rows = 0, cols = 0;
	int col = 0;
	int length = 0;

	setData = dataSet;
	if (!setData) return;

	rows = setData->rows;
	cols = setData->cols;
	length = setData->length;
	begin = setData->elem;
	if (!begin) return;

	end = begin + length;

	for (col = 0; begin != end; begin++, col++)
	{
		if (col >= cols)
		{
			col = 0;
			printf("\n");
		}
		printf(PRNFORMAT, *begin); printf(" ");
	}
	printf("\n");
}

static void SortQuick(EType *dataSet, int posLeft, int posRight)
{
	int pivot, l_hold, r_hold;
	if (posLeft == posRight)
		return;

	l_hold = posLeft;
	r_hold = posRight;
	pivot = dataSet[posLeft];

	while (posLeft < posRight)
	{
		while ((dataSet[posRight] >= pivot) && (posLeft < posRight))
			posRight--;

		if (posLeft != posRight)
		{
			dataSet[posLeft] = dataSet[posRight];
			posLeft++;
		}

		while ((dataSet[posLeft] <= pivot) && (posLeft < posRight))
			posLeft++;

		if (posLeft != posRight)
		{
			dataSet[posRight] = dataSet[posLeft];
			posRight--;
		}
	}

	dataSet[posLeft] = pivot;
	pivot = posLeft;
	posLeft = l_hold;
	posRight = r_hold;

	if (posLeft < pivot)
		SortQuick(dataSet, posLeft, pivot - 1);
	if (posRight > pivot)
		SortQuick(dataSet, pivot + 1, posRight);
}
