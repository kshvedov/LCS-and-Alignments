#include "FullCellList.h"

FullCellList* init_full_cell_list(void)
{
	FullCellList* new_list = (FullCellList*)malloc(sizeof(FullCellList));

	if (new_list != NULL)
	{
		new_list->pHead = NULL;
		new_list->pTail = NULL;
	}

	return new_list;
}


bool insert_cell(FullCellList* list, struct DP_cellFull* node)
{

	if (list != NULL && node != NULL)
	{
		if (list->pHead == NULL)
		{
			list->pHead = node;
		}
		else
		{
			list->pTail->pNext = node;
			node->pPrev = list->pTail;
		}
		list->pTail = node;
		return true;
	}
	return false;
}


void EmplaceBack(FullCellList* l1, FullCellList* l2)
{
	if (l1->pHead == NULL)
	{
		l1->pHead = l2->pHead;
		l2->pTail = l2->pTail;
	}
	else
	{
		l1->pTail->pNext = l2->pHead;
		l2->pHead->pPrev = l1->pTail;
		l1->pTail = l2->pTail;
	}
}