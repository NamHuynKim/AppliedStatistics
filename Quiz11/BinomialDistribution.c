#include <stdio.h>

#ifndef BOOL
typedef int BOOL;
#define FALSE 0
#define TRUE !FALSE
#endif

static double ComputeFactorial(const double f);
static double ComputePermutation(const double n, const double r);
static double ComputeCombination(const double n, const double r);
static double ComputeBinomialDist(const double success, const double trial, const double prob, const BOOL cumulative);
static double ComputeMeanBinomialDist(const double trial, const double prob);
static double ComputeVarBinomialDist(const double trial, const double prob);
static double pow(double x, int y);

int main(void)
{
	int success;
	double trial;
	double prob;
	BOOL cumulative;

	success = 5;
	trial = 20;
	prob = 0.2;
	cumulative = TRUE;

	printf("Number_s = %d \n", success);
	printf("Trials = %2.0f \n", trial);
	printf("Probability_s = %0.1f \n", prob);
	if(cumulative)
		printf("Cumulative = True \n\n");
	else
		printf("Cumulative = False \n\n");
	
	printf("Binomial Distribution Probability \n");
	printf(" = %f \n", ComputeBinomialDist(success, trial, prob, cumulative));
	printf("Mean of Binomial Distribution \n");
	printf(" =%2.0f \n", ComputeMeanBinomialDist(trial, prob));
	printf("Variance of Binomial Distribution \n");
	printf(" = %0.1f \n", ComputeVarBinomialDist(trial, prob));

	return 0;
}

static double ComputeFactorial(const double f)
{
	int i;
	int result = 1;

	for (i = f; i > 0; --i)
		result *= i;

	return result;
}

static double ComputePermutation(const double n, const double r)
{
	int i;
	int result = 1;

	for (i = n; i > n - r; --i)
	{
		result *= i;
	}

	return result;
}

static double ComputeCombination(const double n, const double r)
{
	return ComputePermutation(n, r) / ComputeFactorial(r);
}

static double ComputeBinomialDist(const double success, const double trial, const double prob, const BOOL cumulative)
{
	int i;
	double result;
	
	if (!cumulative)
	{
		result = ComputeCombination(trial, success) * pow(prob, success) * pow(1 - prob, trial - success);
	}
	else
	{
		result = 0;
		for (i = success; i >= 0; --i)
		{
			result += ComputeCombination(trial, i) * pow(prob, i) * pow(1 - prob, trial - i);
		}
	}

	return result;
}

static double ComputeMeanBinomialDist(const double trial, const double prob)
{
	return trial * prob;
}

static double ComputeVarBinomialDist(const double trial, const double prob)
{
	return trial * prob * (1 - prob);
}

static double pow(const double x, const int y)
{
	int i;
	double result = 1;

	for (i = y; i > 0; --i)
	{
		result *= x;
	}

	return result;
}