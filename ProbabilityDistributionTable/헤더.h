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