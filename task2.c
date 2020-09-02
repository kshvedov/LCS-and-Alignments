#include "task2.h"
int GetCellMax(DP_cell* cell)
{
	int max = cell->substitutionScore;
	if (cell->deletionScore > max)
		max = cell->deletionScore;
	if (cell->insertionScore > max)
		max = cell->insertionScore;
	return max;
}


DP_cell* FillInCell(int row, int col, int subScore, int delScore, int insScore, DP_cell* table[])
{
	table[row][col].substitutionScore = subScore;
	table[row][col].deletionScore = delScore;
	table[row][col].insertionScore = insScore;
	table[row][col].isSet = true;
	return &table[row][col];
}

DP_cell* CalculateCell(int row, int col, int h, int g, int match, int mismatch, DP_cell** table, char* s1, char* s2)
{
	int minValue = INT_MIN - h - g + 1;

	if (row == 0 && col == 0)
		return FillInCell(row, col, 0, minValue, minValue, table);
	if (row == 0)
		return FillInCell(row, col, minValue, h + col * g, minValue, table);
	if (col == 0)
		return FillInCell(row, col, minValue, minValue, h + row * g, table);

	int subScore = GetMaxSubScore(row, col, h, g, match, mismatch, table, s1, s2);
	int delScore = GetMaxDeletionScore(row, col, h, g, match, mismatch, table, s1, s2);
	int insScore = GetMaxInsertionScore(row, col, h, g, match, mismatch, table, s1, s2);

	return FillInCell(row, col, subScore, delScore, insScore, table);
}

DP_cell* GetCalculatedCell(int row, int col, int h, int g, int match, int mismatch, DP_cell** table, char* s1, char* s2)
{
	DP_cell* cell = &table[row][col];
	if (cell == NULL)
		cell = CalculateCell(row, col, h, g, match, mismatch, table, s1, s2);
	return cell;
}

int GetMaxSubScore(int row, int col, int h, int g, int match, int mismatch, DP_cell** table, char* s1, char* s2)
{
	int matchScore = mismatch;
	if (s1[row - 1] == s2[col - 1])
		matchScore = match;

	DP_cell* diagCell = GetCalculatedCell(row - 1, col - 1, h, g, match, mismatch, table, s1, s2);
	while (!diagCell->isSet)
	{
	}
	return GetCellMax(diagCell) + matchScore;
}
int GetMaxDeletionScore(int row, int col, int h, int g, int match, int mismatch, DP_cell** table, char* s1, char* s2)
{
	DP_cell* upCell = GetCalculatedCell(row - 1, col, h, g, match, mismatch, table, s1, s2);
	while (!upCell->isSet)
	{
	}
	int max = upCell->deletionScore + g;

	if (upCell->insertionScore + h + g > max)
		max = upCell->insertionScore + h + g;
	if (upCell->substitutionScore + h + g > max)
		max = upCell->substitutionScore + h + g;

	return max;
}

int GetMaxInsertionScore(int row, int col, int h, int g, int match, int mismatch, DP_cell** table, char* s1, char* s2)
{
	DP_cell* leftCell = GetCalculatedCell(row, col - 1, h, g, match, mismatch, table, s1, s2);
	while (!leftCell->isSet)
	{
	}
	int max = leftCell->deletionScore + h + g;

	if (leftCell->insertionScore + g > max)
		max = leftCell->insertionScore + g;
	if (leftCell->substitutionScore + h + g > max)
		max = leftCell->substitutionScore + h + g;

	return max;
}

int GetAlignmentValue(char* s1, int s1Length, char* s2, int s2Length, int h, int g, int match, int mismatch)
{
	DP_cell** table = (DP_cell**)malloc(sizeof(DP_cell*) * (s1Length + 1));
	for (int i = 0; i < s1Length + 1; i++)
		table[i] = (DP_cell*)malloc(sizeof(DP_cell) * (s2Length + 1));
	
	for (int row = 0; row <= s1Length; row++)
		for (int col = 0; col <= s2Length; col++)
			CalculateCell(row, col, h, g, match, mismatch, table, s1, s2);
	
	//PrintTable(table, s1Length + 1, s2Length + 1);

	int value = TraceBackGlobal(s1Length, s2Length,s1, s2, table, h);
	
	for (int i = 0; i < s1Length + 1; i++)
	{
		free(table[i]);
	}
	
	return value;
	
}

int GetAlignmentValueParallel(char* s1, int s1Length, char* s2, int s2Length, int h, int g, int match, int mismatch, int threads)
{
	DP_cell** table = (DP_cell**)malloc(sizeof(DP_cell*) * (s1Length + 1));
	for (int i = 0; i < s1Length + 1; i++)
	{
		table[i] = (DP_cell*)malloc(sizeof(DP_cell) * (s2Length + 1));
		for (int j = 0; j < s2Length + 1; j++)
		{
			table[i][j].isSet = false;
		}
	}
	omp_set_num_threads(threads);
	#pragma omp parallel for schedule(dynamic, 1)
	for (int row = 0; row < s1Length+1; row++)
	{
		for (int col = 0; col <= s2Length; col++)
		{
			CalculateCell(row, col, h, g, match, mismatch, table, s1, s2);
		}
	}

	int value = TraceBackGlobal(s1Length, s2Length, s1, s2, table, h);

	for (int i = 0; i < s1Length + 1; i++)
	{
		free(table[i]);
	}

	return value;

}

void PrintTable(DP_cell** table, int rows, int cols)
{
	printf("\n");
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			DP_cell cell = table[i][j];
			printf("(%d)-(%d)-(%d)\t", cell.insertionScore, cell.substitutionScore, cell.deletionScore);
		}
		printf("\n");
	}

}

FullCellList* GetMaxAdjacentCells(int row, int col, enum Direction prevDirection, DP_cell** table, int h)
{
	FullCellList* maxAdjacentSquares = init_full_cell_list();
	DP_cell* cell = (DP_cell *)malloc(sizeof(DP_cell));
	cell->deletionScore = table[row][col].deletionScore;
	cell->insertionScore = table[row][col].insertionScore;
	cell->substitutionScore = table[row][col].substitutionScore;
	cell->isSet = table[row][col].isSet;
	if (prevDirection == left)
	{
		cell->deletionScore += h;
		cell->substitutionScore += h;
	}
	if (prevDirection == up)
	{
		cell->insertionScore += h;
		cell->substitutionScore += h;
	}
	int max = GetCellMax(cell);

	if (row == 0 && col == 0)
		return init_full_cell_list();
	if (row == 0)
	{
		DP_CellFull* cell = malloc(sizeof(DP_CellFull));
		cell->cell = &table[row][col - 1];
		cell->col = col - 1;
		cell->row = row;
		cell->max = GetCellMax(cell->cell);
		insert_cell(maxAdjacentSquares, cell);
		return maxAdjacentSquares;
	}
	if (col == 0)
	{
		DP_CellFull* cell = malloc(sizeof(DP_CellFull));
		cell->cell = &table[row - 1][col];
		cell->col = col;
		cell->row = row - 1;
		cell->max = GetCellMax(cell->cell);
		insert_cell(maxAdjacentSquares, cell);
		return maxAdjacentSquares;
	}
	if (cell->deletionScore == max)
	{
		DP_CellFull* cell = malloc(sizeof(DP_CellFull));
		cell->cell = &table[row - 1][col];
		cell->col = col;
		cell->row = row - 1;
		cell->max = GetCellMax(cell->cell);
		insert_cell(maxAdjacentSquares, cell);
	}
	if (cell->insertionScore == max)
	{
		DP_CellFull* cell = malloc(sizeof(DP_CellFull));
		cell->cell = &table[row][col - 1];
		cell->col = col - 1;
		cell->row = row;
		cell->max = GetCellMax(cell->cell);
		insert_cell(maxAdjacentSquares, cell);
	}
	if (cell->substitutionScore == max)
	{
		DP_CellFull* cell = malloc(sizeof(DP_CellFull));
		
		cell->cell = &table[row - 1][col - 1];
		cell->col = col - 1;
		cell->row = row - 1;
		cell->max = GetCellMax(cell->cell);
		insert_cell(maxAdjacentSquares, cell);
	}

	return maxAdjacentSquares;
}

int TraceBackGlobal(int row, int col, char* s1, char* s2, DP_cell** table, int h)
{
	if (row <= 0 || col <= 0)
		return 0;
	
	DP_CellFull* mainMaxCell = malloc(sizeof(DP_CellFull));
	mainMaxCell->max = INT_MIN;
	int maxCellSubs = 0;

	enum Direction prevDirection = diag;
	do
	{
		const DP_CellFull* maxCell = GetMaxAdjacentCells(row, col, prevDirection, table, h)->pHead;
		if (maxCell->max > mainMaxCell->max)
		{
			mainMaxCell = maxCell;
			maxCellSubs = 0;
		}
		
		if (maxCell->row < row && maxCell->col < col)
		{
			prevDirection = diag;
			if (s1[row - 1] == s2[col - 1])
				maxCellSubs++;
		}
		else if (maxCell->row < row)
		{
			prevDirection = up;
		}
		else
		{
			prevDirection = left;
		}
		row = maxCell->row;
		col = maxCell->col;
	} while (row > 0 || col > 0);

	return maxCellSubs;
}
