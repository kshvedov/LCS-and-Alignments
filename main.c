#include "parameters.h"
#include "fileReader.h"
#include "st.h"
#include "similarityMatrix.h"
#include "task2.h"
int main(int argc, const char* argv[])
{
    Parameters* params = get_parameters(argc, argv);
    Sequence** seqArray = (Sequence**)malloc(sizeof(Sequence*) * params->inputFileCount);

    for (int i = 0; i < params->inputFileCount; i++)
    {
        printf("processing file %d\n", i);
        Sequence* sequence = get_sequence(params->inputFileNames[i]);
        clean_dna_seq(sequence);
        seqArray[i] = sequence;
    }
    Compute_Similarity_Matrix(seqArray, params->inputFileCount, params->threads);
	
    return 0;
}
