#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PATH_FILE "Sample.dat"

#ifndef DATATYPE
#define DATATYPE
#define SCANFORMAT "%f"
#define PRNFORMAT "%7.3f "
typedef float EType;
#endif

#ifndef MULTIVRAIATE
#define MULTIVRAIATE
typedef struct
{
	int rows;
	int cols;
	char **headers;
	int length;
	float SDx, SDy;
	float meanX, meanY;
	float covariance;
	float correlation;
	float slope;
	float intercept;
	EType **elem;
} MultiVariateSet;
#endif

static MultiVariateSet *CreateMultiVariateSet(const char *filename);
static void DestroyMultiVariateSet(MultiVariateSet *variateSet);
static void PrintMultiVariateSet(MultiVariateSet *variateSet);
static float ComputeCovariance(MultiVariateSet *variateSet);
static float ComputeCorrelation(MultiVariateSet *variateSet);
static void ComputeSlope(MultiVariateSet *variateSet);

int main(int argc, char *argv[])
{
	MultiVariateSet *setVariate = NULL;
	setVariate = CreateMultiVariateSet(PATH_FILE);
	PrintMultiVariateSet(setVariate);
	printf("\n");
	printf("Covariance = %6.3f\n", ComputeCovariance(setVariate));
	printf("Correlation = %6.3f\n", ComputeCorrelation(setVariate));
	printf("\n");
	ComputeSlope(setVariate);
	printf("Slope = %6.3f\n", setVariate->slope);
	printf("Intercept = %6.3f\n", setVariate->intercept);
	DestroyMultiVariateSet(setVariate);
	system("pause");
	return 0;
}

static MultiVariateSet *CreateMultiVariateSet(const char *filename)
{
	MultiVariateSet *MVS = (MultiVariateSet*)malloc(sizeof(MultiVariateSet));
	FILE *infile = NULL;
	EType *begin = NULL, *end = NULL;
	int i, j;
	int meanX = 0, meanY = 0;
	infile = fopen(filename, "r");

	fscanf(infile, "%d %d %d", &MVS->rows, &MVS->cols, &MVS->length);
	MVS->headers = (char**)malloc(sizeof(char*) * 2);
	MVS->headers[0] = (char*)malloc(sizeof(char) * 3);
	MVS->headers[1] = (char*)malloc(sizeof(char) * 3);
	MVS->elem = (EType**)malloc(sizeof(EType*) * MVS->rows);
	MVS->elem[0] = (EType*)malloc(sizeof(EType) * MVS->cols);
	MVS->elem[1] = (EType*)malloc(sizeof(EType) * MVS->cols);

	fgets(MVS->headers[0], 3, infile);
	fgets(MVS->headers[0], 4, infile);
	fgets(MVS->headers[1], 3, infile);

	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 6; j++)
		{
			fscanf(infile, "%f", &MVS->elem[i][j]);
		}
	}



	return MVS;
}

static void DestroyMultiVariateSet(MultiVariateSet *variateSet)
{

}

static void PrintMultiVariateSet(MultiVariateSet *variateSet)
{
	int i;
	printf("----------------------------------------\n");

	for (i = 0; i < variateSet->cols; i++)
	{
		printf(PRNFORMAT, variateSet->elem[0][i]);
		printf(PRNFORMAT, variateSet->elem[1][i]);
		printf("\n");
	}
}

static float ComputeCovariance(MultiVariateSet *variateSet)
{
	float covariance = 0.0;
	float sum = 0.0;
	float meanX = 0.0, meanY = 0.0;
	int i, j;

	for (i = 0; i < variateSet->cols; i++)
	{
		meanX += variateSet->elem[0][i];
		meanY += variateSet->elem[1][i];
	}
	meanX /= 6;
	meanY /= 6;
	variateSet->meanX = meanX;
	variateSet->meanY = meanY;

	for (int i = 0; i < variateSet->cols; i++)
	{
		covariance += (variateSet->elem[0][i] - meanX)*(variateSet->elem[1][i] - meanY);
	}
	covariance /= 6;
	variateSet->covariance = covariance;

	return covariance;
}

static float ComputeCorrelation(MultiVariateSet *variateSet)
{
	float correlation = 0.0;
	float SDx = 0.0, SDy = 0.0;
	int i;

	for (i = 0; i < variateSet->cols; i++)
	{
		SDx += (variateSet->elem[0][i] - variateSet->meanX) * (variateSet->elem[0][i] - variateSet->meanX);
		SDy += (variateSet->elem[1][i] - variateSet->meanY) * (variateSet->elem[1][i] - variateSet->meanY);
	}
	SDx = sqrtf(SDx);
	SDy = sqrtf(SDy);
	variateSet->SDx = SDx;
	variateSet->SDy = SDy;
	correlation = (variateSet->covariance * 6) / (SDx * SDy);
	variateSet->correlation = correlation;
	return correlation;
}

static void ComputeSlope(MultiVariateSet *variateSet)
{
	int j = 0;
	float slope = 0.0;
	float intercept = 0.0;

	slope = variateSet->correlation * variateSet->SDy / variateSet->SDx;
	variateSet->slope = slope;
	intercept = variateSet->meanY - (variateSet->slope * variateSet->meanX);
	variateSet->intercept = intercept;

	for (j = 0; j < 6; j++)
	{
		printf("y = %6.3f \n", slope * variateSet->elem[0][j] + intercept);
	}
}