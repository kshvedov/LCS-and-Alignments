#ifndef SIMILARITYMATRIX_H
#define SIMILARITYMATRIX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "st.h"
#include "fileReader.h"
#include "task2.h"

typedef struct concatSequence
{
	char* str;
	int* starIndexes;
	int starIndexCount;
	int strLength;
} ConcatSequence;

typedef struct simValue
{
	int A;
	int B;
	int C;
	int Total;
}SimValue;

SimValue* Compute_Similarity_Matrix(Sequence** seqArray, int seqNum, int threads);
void Print_Simularity_Matrix(SimValue* matrix, int seqNum);
char* Get_String_Reverse(char* str, int length);
ConcatSequence* BuildConcatSequence(Sequence** seqArray, int seqNum);
#endif