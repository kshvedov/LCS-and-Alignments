#ifndef FULLCELLLIST_H
#define FULLCELLLIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct dP_cell {
	int substitutionScore;
	int deletionScore;
	int insertionScore;
	bool isSet;
} DP_cell;

enum Direction
{
	up,
	left,
	diag
};

typedef struct DP_cellFull {
	DP_cell* cell;
	int row;
	int col;
	int max;
	struct DP_cellFull* pNext;
	struct DP_cellFull* pPrev;
} DP_CellFull;

typedef struct fullCellList
{
	struct DP_cellFull* pHead;
	struct DP_cellFull* pTail;
} FullCellList;

FullCellList* init_full_cell_list(void);
void EmplaceBack(FullCellList* l1, FullCellList* l2);
bool insert_cell(FullCellList* list, struct DP_cellFull* node);

#endif