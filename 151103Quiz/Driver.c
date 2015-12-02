#include <stdio.h>
#include <stdlib.h>

typedef struct setstrings SetStrings;
typedef struct setchars SetChars;

struct setchars {
	int size;
	char* elems;
};
struct setstrings {
	int size;
	SetChars* elems;
};

int GetCardinality(SetChars* set);
int Combination(int n, int r);
int Permutation(int n, int r);
SetStrings *MakeCombination(SetChars *set, const int r);
SetStrings *MakePermutation(SetChars *set, const int r);
void PrintSetStrings(SetStrings* set);
void PrintSetChars(SetChars* set);
void DestroySetStrings(SetStrings* set);
void DestroyChars(SetChars* set);
void Swap(char *elem, int i, int j);

int main(void)
{
	SetChars setChars = { 3, "abc" };
	SetStrings *permutationStrings = NULL;
	SetStrings *combinationStrings = NULL;

	permutationStrings = MakePermutation(&setChars, 2);
	combinationStrings = MakeCombination(&setChars, 3);

	printf("Permutation Strings = \n");
	PrintSetStrings(permutationStrings);
	//printf("Combination Strings = \n");
	//PrintSetStrings(combinationStrings);

	DestroySetStrings(permutationStrings);
	DestroySetStrings(combinationStrings);

	return 0;
}

int GetCardinality(SetChars* set)
{
	return set->size;
}

int Combination(int n, int r)
{
	int i;
	int total = Permutation(n, r);

	for (i = r; i > 0; --i)
		total /= i;

	return total;
}

int Permutation(int n, int r)
{
	int total = 1;
	int i;

	for (i = n; i >= n-r; --i)
		total *= i;
		
	return total;
}

SetStrings *MakeCombination(SetChars *set, const int r)
{
	int i, j;
	int k;
	int count;

	SetStrings *tempStrings = (SetStrings*)malloc(sizeof(SetStrings));
	tempStrings->size = Combination(set->size, r);
	tempStrings->elems = (SetChars*)malloc(sizeof(SetChars) * tempStrings->size);

	for (i = 0; i < tempStrings->size; ++i)
	{
		tempStrings->elems[i].size = r;
		tempStrings->elems[i].elems = (char*)malloc(sizeof(char) * (r + 1));
		k = i % set->size;
		count = 0;
		for (j = 0; j < set->size; ++j)
		{
			if (j != k)
			{
				tempStrings->elems[i].elems[count++] = set->elems[j];
				tempStrings->elems[i].elems[count] = '\0';
			}
		}
	}
}

SetStrings *MakePermutation(SetChars *set, const int r)
{
	int i;
	int k;
	SetStrings *tempStrings = (SetStrings*)malloc(sizeof(SetStrings));
	
	tempStrings->size = Permutation(set->size, r);
	tempStrings->elems = (SetChars*)malloc(sizeof(SetChars) * tempStrings->size);

	tempStrings->elems[0].elems = (char*)malloc(sizeof(char)*(set->size + 1));
	tempStrings->elems[0].size = set->size;
	strcpy(tempStrings->elems[0].elems, set->elems);

	for (i = 1; i < tempStrings->size; i++)
	{
		tempStrings->elems[i].size = set->size;
		tempStrings->elems[i].elems = (char*)malloc(sizeof(char)*(set->size + 1));
		strcpy(tempStrings->elems[i].elems, tempStrings->elems[i-1].elems);

		k = i % (set->size-1);
		if (k == 0) k = set->size - 1;

		Swap(tempStrings->elems[i].elems, 0, k);
	}
}

void PrintSetStrings(SetStrings* set)
{
	int i;

	for (i = 0; i < set->size; i++)
	{
		printf("%d. ", i+1);
		PrintSetChars(&set->elems[i]);
	}
	printf("\n");
}

void PrintSetChars(SetChars* set)
{
	printf("%s\n", set->elems);
}

void DestroySetStrings(SetStrings* set)
{
	int i;

	if (set == NULL)
		return;

	for (i = 0; i < set->size; i++)
	{
		DestroyChars(&set->elems[i]);
	}
	free(set->elems);
	free(set);
}

void DestroyChars(SetChars* set)
{
	if (set == NULL)
		return;
	free(set->elems);
}

void Swap(char *elem, int i, int j)
{
	char temp;
	temp = elem[i];
	elem[i] = elem[j];
	elem[j] = temp;
}