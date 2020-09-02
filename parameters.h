#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#pragma warning(disable:4996)

typedef struct parameters
{
	int inputFileCount;
	char** inputFileNames;
	char* alphabetFileName;
	int threads;

} Parameters;

Parameters* get_parameters(int argc, const char* argv[]);

#endif