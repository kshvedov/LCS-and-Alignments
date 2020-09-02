#ifndef TASK2_H
#define TASK2_H
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "omp.h"
#include "FullCellList.h"


DP_cell* FillInCell(int row, int col, int subScore, int delScore, int insScore, DP_cell* table[]);
DP_cell* CalculateCell(int row, int col, int h, int g, int match, int mismatch, DP_cell** table, char* s1, char* s2);
DP_cell* GetCalculatedCell(int row, int col, int h, int g, int match, int mismatch, DP_cell** table, char* s1, char* s2);
int GetMaxSubScore(int row, int col, int h, int g, int match, int mismatch, DP_cell** table, char* s1, char* s2);
int GetMaxDeletionScore(int row, int col, int h, int g, int match, int mismatch, DP_cell** table, char* s1, char* s2);
int GetMaxInsertionScore(int row, int col, int h, int g, int match, int mismatch, DP_cell** table, char* s1, char* s2);
int GetAlignmentValue(char* s1, int s1Length, char* s2, int s2Length, int h, int g, int match, int mismatch);
int GetCellMax(DP_cell* cell);
void PrintTable(DP_cell** table, int rows, int cols);
int TraceBackGlobal(int row, int col, char* s1, char* s2, DP_cell** table, int h);
int TraceBackGlobalLean(int row, int col, DP_cell** table, int h);
FullCellList* GetMaxAdjacentCells(int row, int col, enum Direction prevDirection, DP_cell** table, int h);
int GetAlignmentValueParallel(char* s1, int s1Length, char* s2, int s2Length, int h, int g, int match, int mismatch, int threads);
#endif

