#include "similarityMatrix.h"

SimValue* Compute_Similarity_Matrix(Sequence** seqArray, int seqNum, int threads)
{
	ConcatSequence* c = BuildConcatSequence(seqArray, seqNum);

	SimValue* similarityMatix = malloc(sizeof(SimValue) * (seqNum * (seqNum - 1) / 2));
	clock_t task1Start = clock();
	
	Node* root = Build_GSTree(c->str, c->starIndexes, c->starIndexCount);
	FILE* fingerPrintFile = fopen("FingerPrints.txt", "w");
	Fingerprint* fingerPrints = malloc(sizeof(Fingerprint) * seqNum);
	for (int i = 0; i < seqNum; i++)
	{
		fingerPrints[i].size = INT_MAX;
		fingerPrints[i].pHead = NULL;
		fingerPrints[i].sequence = seqArray[i];
	}
	clock_t fingerPrintTimeStart = clock();
	GetFingerPrints(root, fingerPrints, mix_colour);
	clock_t fingerPrintTimeEnd = clock();
	fprintf(fingerPrintFile, "\nFingerprint time: %f\n", ((double)(fingerPrintTimeEnd - fingerPrintTimeStart)) / CLOCKS_PER_SEC);
	for (int i = 0; i < seqNum; i++)
	{
		fingerPrints[i].str = malloc(sizeof(char) * fingerPrints[i].size+1);
		NodeListNode* curNode = fingerPrints[i].pHead;
		int curLocation = 0;
		while (curNode->pNext != NULL)
		{
			for (int j = curNode->myNode->i; j <= curNode->myNode->j; j++)
				fingerPrints[i].str[curLocation++] = c->str[j];
			curNode = curNode->pNext;
		}
		fingerPrints[i].str[curLocation++] = c->str[curNode->myNode->i];
		fingerPrints[i].str[curLocation++] = '\0';
		fprintf(fingerPrintFile, "\nFingerprint for sequence %d is %d long\n%s\n", i, fingerPrints[i].size, fingerPrints[i].str);
	}
	clock_t task1End = clock();
	printf("\nTask1 Total Time %f\n", ((double)(task1End - task1Start)) / CLOCKS_PER_SEC);
	Destroy_Tree(root);

	double alignmentTime = 0;
	double suffixTime = 0;
	clock_t task2Start = clock();
	for (int i = 0; i < seqNum; i++)
	{
		for (int j = i + 1; j < seqNum; j++)
		{
			printf("\n\nAlignment %d:%d\n", i, j);
			char* seq1 = (char*)malloc((seqArray[i]->len_str + 2) * sizeof(char));
			char* seq2 = (char*)malloc((seqArray[j]->len_str + 2) * sizeof(char));

			strcpy(seq1, seqArray[i]->str);
			strcpy(seq2, seqArray[j]->str);

			seq1[seqArray[i]->len_str] = '$';
			seq1[seqArray[i]->len_str + 1] = '\0';
			seq2[seqArray[j]->len_str] = '$';
			seq2[seqArray[j]->len_str + 1] = '\0';

			char* l_seq = (char*)malloc((strlen(seq1) + strlen(seq2) + 10000) * sizeof(char));
			strcpy(l_seq, seq1);
			strcat(l_seq, seq2);

			int temp_seq[2] = { 0, seqArray[i]->len_str + 1 };
			clock_t startGST = clock();
			
			Node* gstHead = Build_GSTree(l_seq, temp_seq, 2);
			
			clock_t EndGST = clock();
			suffixTime += ((double)(EndGST - startGST)) / CLOCKS_PER_SEC;

			LcsCoordinate* lcs = Get_LCS(gstHead);
			printf("\nLCS Length:%d", lcs->x2 - lcs->x1+1);
			
			printf("\nLCS:");
			for (int i = lcs->x1; i <= lcs->x2; i++)
				printf("%c", l_seq[i]);
			printf("\n");
			//your stuff here
			char* s1StartReversed = Get_String_Reverse(seq1, lcs->x1);
			char* s2StartReversed = Get_String_Reverse(seq2, lcs->y1 - temp_seq[1]);

			char* s1EndStart = seq1 + lcs->x2 + 1;
			int s1EndStartLength = seqArray[i]->len_str - lcs->x2 - 1;
			char* s2EndStart = seq2 + lcs->y2 - temp_seq[1] + 1;
			int s2EndStartLength = seqArray[j]->len_str - (lcs->y2 - temp_seq[1] + 1);
			
			clock_t start = clock();
			int A = GetAlignmentValueParallel(s1StartReversed, lcs->x1 - 1, s2StartReversed, lcs->y1 - temp_seq[1] - 1, -5, -2, 1, -2, threads);
			int B = lcs->x2 - lcs->x1 + 1;

			int C = GetAlignmentValueParallel(s1EndStart, s1EndStartLength, s2EndStart, s2EndStartLength, -5, -2, 1, -2,threads);
			
			clock_t end = clock();

			alignmentTime += ((double)(end - start)) / CLOCKS_PER_SEC/threads;

			//insert a value into matrix
			similarityMatix[(j * (j - 1) / 2) + i].A = A;
			similarityMatix[(j * (j - 1) / 2) + i].B = B;
			similarityMatix[(j * (j - 1) / 2) + i].C = C;
			similarityMatix[(j * (j - 1) / 2) + i].Total = A + B + C;
			printf("Alignment %d:%d took %f and produced a similarity of (%d, %d, %d) total:%d\n", i, j, ((double)(end - start)) / CLOCKS_PER_SEC / threads, A, B, C, A + B + C);
			Destroy_Tree(gstHead);
		}
	}
	clock_t task2End = clock();
	printf("Task2 Total Time: %f\nTask2 GST Time:%f\nTask2 Alignment Time: %f ", ((double)(task2End - task2Start)) / CLOCKS_PER_SEC/threads, suffixTime, alignmentTime);
	Print_Simularity_Matrix(similarityMatix, seqNum);
	return similarityMatix;
	//should return a matrix or should output it, or what ever XD
}


void Print_Simularity_Matrix(SimValue* matrix, int seqNum)
{
	printf("\n");
	printf("\n");
	for (int i = 0; i < seqNum; i++)
	{
		printf("S%d\t", i);
	}
	printf("\n");
	for (int i = 1; i < seqNum; i++)
	{
		printf("S%d", i);
		for (int j = 0; j < i; j++)
		{
			printf("\t%d", matrix[(i * (i - 1) / 2) + j].Total);
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
	for (int i = 0; i < seqNum; i++)
	{
		printf("S%d\t", i);
	}
	printf("\n");
	for (int i = 1; i < seqNum; i++)
	{
		printf("S%d", i);
		for (int j = 0; j < i; j++)
		{
			printf("\t%d", matrix[(i * (i - 1) / 2) + j].B);
		}
		printf("\n");
	}
}

char* Get_String_Reverse(char* str, int length)
{
	char* returnStr = malloc(sizeof(char) * length + 1);
	for (int i = length - 1; i >= 0; i--)
	{
		returnStr[length - 1 - i] = str[i];
	}
	returnStr[length] = '\0';
	return returnStr;
}

ConcatSequence* BuildConcatSequence(Sequence** seqArray, int seqNum)
{
	ConcatSequence* concat = malloc(sizeof(ConcatSequence));
	concat->starIndexCount = seqNum;
	concat->starIndexes = malloc(sizeof(int) * seqNum);

	int length = 0;
	for (int i = 0; i < seqNum; i++)
	{
		length += seqArray[i]->len_str + 1;
	}
	concat->str = malloc(sizeof(char) * length + 1);
	concat->strLength = length;
	int curLocation = 0;
	for (int i = 0; i < seqNum; i++)
	{
		concat->starIndexes[i] = curLocation;
		strcpy(concat->str + curLocation, seqArray[i]->str);
		curLocation += seqArray[i]->len_str;
		concat->str[curLocation++] = '$';
		concat->str[curLocation] = '\0';
	}
	return concat;
}