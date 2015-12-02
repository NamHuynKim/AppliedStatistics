#define PRNFORMAT " %2d"

#ifndef DATASET
#define DATASET

typedef int EType;

typedef struct dataSet
{
	int rows;
	int cols;
	int length;
	EType *elem;
} DataSet;
#endif

#ifndef STEMNODE
#define STEMNODE
typedef struct stemNode
{
	int stem;
	int freq;
	EType *leaf;
	struct stemNode *next;
}StemNode;
#endif

#ifndef STEMPLOTLIST
#define STEMPLOTLIST
typedef struct stemPlotList
{
	StemNode *head;
	StemNode *tail;
} StemPlotList;
#endif