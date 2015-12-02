#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#ifndef BOOL
typedef int BOOL;
#define FALSE 0
#define TRUE !FALSE
#endif

#ifndef DATATYPE
#define DATATYPE
#define SCANFORMAT "%f"
#define PRNFORMAT "%f"
typedef float EType;
#endif

#ifndef DATASET
#define DATASET
typedef struct
{
	int rows;
	int cols;
	int length;
	EType *elem;
} DataSet;
#endif

#ifndef NDIST
#define NDIST(mat, row, col) (mat->elem[row * (mat->cols) + col])
#endif

#define PATH_FILE "input.txt"

static DataSet *CreateNormDist(const int rowsDataSet, const int colsDataSet);
static void DestroyDataSet(DataSet *dataSet);
static void PrintDataSet(DataSet *dataSet);
static float FindNormDist(const DataSet *dataSet, const float value);
static float Standardize(const float mean, const float sd, const float v);
static float ComputeNormDistGt(const DataSet *dataSet, const float mean, const float sd, const float v);
static float ComputeNormDistGtLt(const DataSet *dataSet, const float mean, const float sd, const float v1, const float v2);
static float ComputeNormDistLt(const DataSet *dataSet, const float mean, const float sd, const float v1);

int main(void)
{
	FILE *inFile = NULL;
	DataSet *setData = NULL;
	EType *begin = NULL;
	int rows = 0, cols = 0;
	inFile = fopen(PATH_FILE, "r");
	if (!inFile) 
		abort();
	fscanf(inFile, "%d %d", &rows, &cols);
	setData = CreateNormDist(rows, cols);
	begin = setData->elem;
	while (fscanf(inFile, SCANFORMAT, begin++) != EOF);
	fclose(inFile);

	PrintDataSet(setData);
	printf("\n");
	float mean = 500.f;
	float sd = 5.f;
	float prov = ComputeNormDistGt(setData, mean, sd, 510.f);
	printf("P(X > 510) = %f\n", prov);
	printf("\n");
	prov = ComputeNormDistLt(setData, mean, sd, 498.f);
	printf("P(X < 498) = %f\n", prov);
	printf("\n");
	prov = ComputeNormDistGtLt(setData, mean, sd, 491.f, 498.f);
	printf("P(491 < X < 498) = %f\n", prov);
	printf("\n");
	prov = ComputeNormDistGtLt(setData, mean, sd, 492.f, 514.f);
	printf("P(492 < X < 514) = %f\n", prov);
	printf("\n");
	DestroyDataSet(setData);
}

static DataSet *CreateNormDist(const int rowsDataSet, const int colsDataSet)
{

	DataSet *dataSet = (DataSet*)malloc(sizeof(DataSet));
	dataSet->rows = rowsDataSet;
	dataSet->cols = colsDataSet;
	dataSet->length = rowsDataSet * colsDataSet;
	dataSet->elem = (EType*)malloc(sizeof(EType) * dataSet->length);
	return dataSet;
}

static void DestroyDataSet(DataSet *dataSet)
{
	free(dataSet->elem);
	dataSet->elem = NULL;
	free(dataSet);
	dataSet = NULL;
}

static void PrintDataSet(DataSet *dataSet)
{
	int i, j;

	for (i = 0; i < dataSet->rows; ++i)
	{
		for (j = 0; j < dataSet->cols; ++j)
		{
			printf(PRNFORMAT, dataSet->elem[dataSet->cols * i + j]);
			printf(" ");
		}
	}
	printf("\n");
}

static float FindNormDist(const DataSet *dataSet, const float value)
{
	int z = value * 100;
	return dataSet->elem[z];
}

static float Standardize(const float mean, const float sd, const float v)
{
	return (v - mean) / sd;
}

static float ComputeNormDistGt(const DataSet *dataSet, const float mean, const float sd, const float v)
{
	float z = Standardize(mean, sd, v);
	
	if (z >= 0)
	{
		return 0.5 - FindNormDist(dataSet, z);
	}
	else
	{
		z *= -1.0;
		return 0.5 + FindNormDist(dataSet, z);
	}
}

static float ComputeNormDistGtLt(const DataSet *dataSet, const float mean, const float sd, const float v1, const float v2)
{
	float z1 = Standardize(mean, sd, v1);
	float z2 = Standardize(mean, sd, v2);

	if (z1 < 0 && z2 < 0)
	{
		z1 *= -1.0;
		z2 *= -1.0;

		return FindNormDist(dataSet, z1) - FindNormDist(dataSet, z2);
	}
	else if (z1 < 0 && z2 >= 0)
	{
		z1 *= -1.0;
		return FindNormDist(dataSet, z1) + FindNormDist(dataSet, z2);
	}
	else
	{
		return  FindNormDist(dataSet, z2) - FindNormDist(dataSet, z1);
	}
}

static float ComputeNormDistLt(const DataSet *dataSet, const float mean, const float sd, const float v1)
{
	float z = Standardize(mean, sd, v1);

	if (z >= 0)
	{
		return 0.5 + FindNormDist(dataSet, z);
	}
	else
	{
		z *= -1.0;
		return 0.5 - FindNormDist(dataSet, z);
	}
}