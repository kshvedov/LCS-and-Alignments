#include "parameters.h"

// Description:
// extracts the parameters from the argument line
// Return:
// filled out parameters struct
Parameters* get_parameters(int argc, const char* argv[])
{
	Parameters* params = (Parameters*)malloc(sizeof(Parameters));
	params->inputFileCount = argc - 4;
	params->inputFileNames = malloc(sizeof(char*) * params->inputFileCount);
	params->threads = omp_get_num_procs() - 2;
	if (params->threads < 1)
		params->threads = 1;
	for (int i = 1; i < argc;)
	{
		if (argv[i][0] == '-')
		{
			if (argv[i][1] == 'a' || argv[i][1] == 'A')
			{
				i++;
				params->alphabetFileName = malloc(sizeof(char) * strlen(argv[i]));
				params->alphabetFileName = argv[i];
				i++;
			}
			else if (argv[i][1] == 'i' || argv[i][1] == 'I')
			{
				int j = 0;
				i++;
				while (i < argc && argv[i][0] != '-')
				{
					params->inputFileNames[j] = malloc(sizeof(char) * strlen(argv[i]));
					strcpy(params->inputFileNames[j], argv[i]);
					j++;
					i++;
				}
			}
			else if (argv[i][1] == 't' || argv[i][1] == 'T')
			{
				i++;
				params->threads = atoi(argv[i]);
				i++;
				params->inputFileCount -= 2;
			}
		}
	}
	return params;
}
