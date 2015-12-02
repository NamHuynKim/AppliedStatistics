#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

void PermuteString(char *str, int begin, int end);
void swap(char *des, char *src);

void main()
{
	FILE *infile;
	int i, j;
	char *str = NULL;
	int numCases, n, k;

	infile = fopen("input.txt", "r");
	fscanf(infile, "%d", &numCases);
	for (i = 0; i < numCases; i++)
	{
		fscanf(infile, "%d %d", &n, &k);
		str = (char*)calloc(n+1, sizeof(char) * n + 1);
		for (j = 0; j < n; j++)
			str[j] = 'a' + j;
		PermuteString(str, 0, k);
	}
}

void PermuteString(char *str, int begin, int end)
{
	int i;
	int range = end - begin;
	if (range == 1)
		printf("%s \n", str);
	else
	{
		for (i = 0; i < range; i++)
		{
			swap(&str[begin], &str[begin + i]);
			PermuteString(str, begin + 1, end);
			swap(&str[begin], &str[begin + i]);
		}
	}
}

void swap(char *des, char *src)
{
	char tmp = *des;
	*des = *src;
	*src = tmp;
}