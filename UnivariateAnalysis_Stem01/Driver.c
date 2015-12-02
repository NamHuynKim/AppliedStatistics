#define _CRT_SECURE_NO_WARNINGS

#include "Header.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

static DataSet *CreateDataSet(const char *fileName, const int rowsDataSet, const int colsDataSet, const int lengthDataSet);
static StemNode *CreateStemNode(const int elemSize, const int stemValue);
static StemPlotList *CreateStemPlotList(void);
static void DestroyDataSet(DataSet *dataSet);
static void DestroyStemPlotList(StemPlotList *stemPlotList);
static void DestroyStemNode(StemNode *stemNode);
static void ComputeStemPlotList(StemPlotList *stemPlotList, DataSet * dataSet);
static void AddStemNode(StemPlotList *stemPlotList, StemNode *stemNode);
static void PrintLeftLeaf(StemNode *stemNode);
static void PrintRightLeaf(StemNode *stemNode);
static void PrintStemPlotList(StemPlotList *leftStemPlotList, StemPlotList *rightStemPlotList);
static void PrintData(DataSet *dataSet);
static void SortQuick(EType *dataSet, int posLeft, int posRight);


int main(void)
{
	StemPlotList *stemPlotList1, *stemPlotList2;
	DataSet *dataSet1, *dataSet2;

	dataSet1 = CreateDataSet("hbq.dat", 4, 10, 35);
	dataSet2 = CreateDataSet("nbq.dat", 4, 10, 32);

	printf("Print Left \n");
	PrintData(dataSet1);
	printf("Print Right \n");
	PrintData(dataSet2);

	stemPlotList1 = CreateStemPlotList();
	stemPlotList2 = CreateStemPlotList();

	ComputeStemPlotList(stemPlotList1, dataSet1);
	ComputeStemPlotList(stemPlotList2, dataSet2);

	PrintStemPlotList(stemPlotList1, stemPlotList2);

	DestroyDataSet(dataSet1);
	DestroyDataSet(dataSet2);
	DestroyStemPlotList(stemPlotList1);
	DestroyStemPlotList(stemPlotList2);
	system("pause");

	return 0;
}

static DataSet *CreateDataSet(const char *fileName, const int rowsDataSet, const int colsDataSet, const int lengthDataSet)
{
	FILE *infile;
	DataSet *dataSet = NULL;
	EType *bgn = NULL, *end = NULL;

	infile = fopen(fileName, "r");
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

static void PrintRightLeaf(StemNode *stemNode)
{
	if (!stemNode)
	{
		printf("(  0) : \n");
		return;
	}
	EType *bgn = NULL, *end = NULL;
	bgn = stemNode->leaf;
	end = stemNode->leaf + stemNode->freq;
	printf("( %2d) :", stemNode->freq);
	while (bgn != end)
	{
		printf(" %d", *bgn++);
	}
	printf("\n");
}

static void PrintLeftLeaf(StemNode *stemNode)
{
	int i;
	if (!stemNode)
	{
		for (i = 0; i < 14; i++)
			printf("  ");
		printf(" : (  0)");
		return;
	}

	for (i = 13; i >= 0; i--)
	{
		if (i < stemNode->freq)
			printf(" %d", stemNode->leaf[i]);
		else
			printf("  ");
	}
	printf(" : ( %2d)", stemNode->freq);
}

static void PrintStemPlotList(StemPlotList *leftStemPlotList, StemPlotList *rightStemPlotList)
{
	StemNode *leftBgn, *rightBgn;
	leftBgn = leftStemPlotList->head;
	rightBgn = rightStemPlotList->head;

	printf("\t\t\tLeft :      Stem     : Right \n");
	printf("--------------------------------------------------------------------------------\n");
	while (leftBgn && rightBgn)
	{
		if (leftBgn->stem == rightBgn->stem)
		{
			PrintLeftLeaf(leftBgn);
			printf(" %d ", leftBgn->stem);
			PrintRightLeaf(rightBgn);
			leftBgn = leftBgn->next;
			rightBgn = rightBgn->next;
		}
		else if (leftBgn->stem < rightBgn->stem)
		{
			PrintLeftLeaf(leftBgn);
			printf(" %d ", leftBgn->stem);
			PrintRightLeaf(NULL);
			leftBgn = leftBgn->next;
		}
		else
		{
			PrintLeftLeaf(NULL);
			printf(" %d ", rightBgn->stem);
			PrintRightLeaf(rightBgn);
			rightBgn = rightBgn->next;
		}
	}

	while (leftBgn)
	{
		PrintLeftLeaf(leftBgn);
		printf(" %d ", leftBgn->stem);
		PrintRightLeaf(NULL);
		leftBgn = leftBgn->next;
	}

	while (rightBgn)
	{
		PrintLeftLeaf(NULL);
		printf(" %d ", rightBgn->stem);
		PrintRightLeaf(rightBgn);
		rightBgn = rightBgn->next;
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
