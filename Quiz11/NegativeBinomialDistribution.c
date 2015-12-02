#include <stdio.h>

#ifndef BOOL
typedef int BOOL;
#define FALSE 0
#define TRUE !FALSE
#endif

static double ComputeFactorial(const double f);
static double ComputePermutation(const double n, const double r);
static double ComputeCombination(const double n, const double r);
static double ComputeNegativeBinomialDist(const double failure, const double success, const double prob, const BOOL cumulative);
static double ComputeMeanNegativeBinomialDist(const double success, const double prob);
static double ComputeVarNegativeBinomialDist(const double success, const double prob);
static double pow(double x, int y);

int main(void)
{
	double success;
	double failure;
	double prob;
	BOOL cumulative;

	failure = 2;
	success = 3;
	prob = 0.4;
	cumulative = TRUE;

	printf("Number_f = %2.0f \n", failure);
	printf("Number_s = %2.0f \n", success);
	printf("Probability_s = %0.1f \n", prob);
	if (cumulative)
		printf("Cumulative = True \n\n");
	else
		printf("Cumulative = False \n\n");

	printf("Negative Binomial Distribution Probability \n");
	printf(" = %f \n", ComputeNegativeBinomialDist(failure, success, prob, cumulative));
	printf("Mean of Negative Binomial Distribution \n");
	printf(" = %2.1f \n", ComputeMeanNegativeBinomialDist(success, prob));
	printf("Variance of Negative Binomial Distribution \n");
	printf(" = %0.2f \n", ComputeVarNegativeBinomialDist(success, prob));

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

static double ComputeNegativeBinomialDist(const double failure, const double success, const double prob, const BOOL cumulative)
{
	int i;
	double result;

	if (!cumulative)
	{
		result = ComputeCombination(success + failure - 1, success - 1) * pow(prob, success) * pow(1 - prob, failure);
	}
	else
	{
		result = 0;
		for (i = failure; i >= 0; --i)
			result += ComputeCombination(success + i - 1, success - 1) * pow(prob, success) * pow(1 - prob, i);
	}
	return result;
}

static double ComputeMeanNegativeBinomialDist(const double success, const double prob)
{
	return success / prob;
}

static double ComputeVarNegativeBinomialDist(const double success, const double prob)
{
	return (success * (1 - prob)) / (prob * prob);
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