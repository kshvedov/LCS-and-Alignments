#include "fileReader.h"

// Description:
// extracts an alphabet from an alphabet fil
// Return:
// array of alphabet characters
char* get_alphabet(char* file_name)
{
	FILE* infile = fopen(file_name, "r");

	if (infile == NULL)
	{
		printf("File couldn't be opened\n");
		return NULL;
	}

	// Assuming Ascii is 0-127 weand all chars are seperated by a space
	// We use a temporary holder of 260 chars
	char line[260] = "";

	//since it is one line, we dont have a loop
	if (fgets(line, 260, infile) != NULL)
	{
		int line_len = strlen(line), alp_len = 0;

		// Counts amount of characters in alphabet
		for (int i = 0; i < line_len; i++)
		{
			if (line[i] != ' ')
			{
				alp_len++;
			}
		}

		alp_len++;

		// Creates a string of alphabet size
		char* alphabet = (char*)malloc(alp_len * sizeof(char));
		int j = 0;

		for (int i = 0; i < line_len; i++)
		{
			if (line[i] != ' ')
			{
				alphabet[j++] = line[i];
			}
		}
		alphabet[j] = '\0';

		return alphabet;
	}
	else
	{
		printf("Something went wrong!");
		return NULL;
	}
}

// Description:
// extracts a sequence name and sequence from a file containing a single sequence
// Return:
// filled out sequence object
Sequence* get_sequence(char* file_name)
{
	FILE* infile = fopen(file_name, "r");

	if (infile == NULL)
	{
		printf("Sequence File couldn't be opened\n");
		return false;
	}

	char line[1000] = "";
	char copy_line[1000] = "";
	bool reading_seq = false;

	// read file the first time to remember length of sequence for correct allocation
	// in second read through
	Sequence* new_seq = NULL;
	new_seq = init_sequence();
	while (fgets(line, 1000, infile) != NULL)
	{
		strcpy(copy_line, line);

		if (copy_line[0] == '>')
		{

			reading_seq = true;
			char* str_start = NULL;
			int i = 1;

			while (copy_line[i] != ' ' && copy_line[i] != '\n')
			{
				new_seq->str_name_len++;
				i++;
			}

			copy_line[i] = '\0';
			str_start = &copy_line[1];

			char* name = (char*)malloc((new_seq->str_name_len + 1) * sizeof(char));
			strcpy(name, str_start);
			new_seq->str_name = name;

			//printf("%s, %d\n", (new_seq->str_name), (new_seq->str_name_len));
		}
		else if (((copy_line[0] >= 65 && copy_line[0] <= 90) || (copy_line[0] >= 97 &&
			copy_line[0] <= 122)) && reading_seq == true)
		{
			int i = 0;
			int len = strlen(copy_line);
			while (copy_line[i] != '\n' && i < len)
				i++;

			new_seq->len_str += i;
		}
		else
		{
			reading_seq = false;
			if (new_seq->len_str == 0)
			{
				fclose(infile);
				return false;
			}
		}
	}
	fseek(infile, 0, SEEK_SET);

	while (fgets(line, 1000, infile) != NULL || new_seq != NULL || !feof(infile))
	{
		strcpy(copy_line, line);

		if (copy_line[0] == '>')
		{
			reading_seq = true;
			char* str_start = NULL;
			char* seq = (char*)malloc((new_seq->len_str + 1) * sizeof(char));
			seq[0] = '\0';
			new_seq->str = seq;
		}
		else if (((copy_line[0] >= 65 && copy_line[0] <= 90) || (copy_line[0] >= 97 &&
			copy_line[0] <= 122)) && reading_seq == true)
		{
			int i = 0;
			int len = strlen(copy_line);
			while (copy_line[i] != '\n' && i < len)
				i++;
			if (i < len)
				copy_line[i] = '\0';

			//for some reason, last line of file is read multiple times if file
			//is reopened or lseek is used in the same function
			if (strlen(new_seq->str) < new_seq->len_str)
				strcat(new_seq->str, copy_line);
			else
				break;
		}
		else if (copy_line[0] == '\n')
		{
			//printf("%s\n", (temp->str));
			reading_seq = false;
			break;
			//new_seq = new_seq->pNext;
		}
	}

	//printf("%s\n", (temp->str));


//printf("%s\n", (temp->str));
	fclose(infile);

	return new_seq;
}

// Description:
// creates a new sequence object and initialises it
// Return:
// Initialised sequence object
Sequence* init_sequence(void)
{
	Sequence* new_seq = (Sequence*)malloc(sizeof(Sequence));

	if (new_seq != NULL)
	{
		new_seq->str_name = NULL;
		new_seq->str_name_len = 0;
		new_seq->str = NULL;
		new_seq->len_str = 0;

		new_seq->pNext = NULL;
		new_seq->pPrev = NULL;
	}

	return new_seq;
}

// Description:
// creates a new sequence list
// Return:
// Initialised list
Seq_List* init_list(void)
{
	Seq_List* new_list = (Seq_List*)malloc(sizeof(Seq_List));

	if (new_list != NULL)
	{
		new_list->pHead = NULL;
		new_list->pTail = NULL;
	}

	return new_list;
}

// Description:
// Inserts a new sequence node into list
// Return:
// 0 if not inserted, 1 if inserted
bool insert_seq(Seq_List* list, Sequence* seq_node)
{

	if (list != NULL && seq_node != NULL)
	{
		if (list->pHead == NULL)
		{
			list->pHead = seq_node;
		}
		else
		{
			list->pTail->pNext = seq_node;
			seq_node->pPrev = list->pTail;
		}
		list->pTail = seq_node;
		return true;
	}
	return false;
}

// Description:
// picks a random char value from array
// Return:
// returns char selected
char rand_val_picker(char arr[], int size)
{
	srand(time(NULL));
	int val = rand() % size;
	return arr[val];
}

// Description:
// removes characters other than ACGT and replaces with value
// Return:
// NA/changes the sequence
void clean_dna_seq(Sequence* s)
{
	char c;
	char arrR[] = { 'A', 'G' }; //ignore this error
	char arrY[] = { 'C', 'T' }; //ignore this error
	char arrK[] = { 'G', 'T' }; //ignore this error
	char arrM[] = { 'A', 'C' }; //ignore this error
	char arrS[] = { 'C', 'G' }; //ignore this error
	char arrW[] = { 'A', 'T' }; //ignore this error
	char arrB[] = { 'C', 'G', 'T' }; //ignore this error
	char arrD[] = { 'A', 'G', 'T' }; //ignore this error
	char arrH[] = { 'A', 'C', 'T' }; //ignore this error
	char arrV[] = { 'A', 'C', 'G' }; //ignore this error
	char arrN[] = { 'A', 'G', 'C', 'T' }; //ignore this error


	
	for (int i = 0; i < s->len_str; i++)
	{
		c = s->str[i];
		
		switch (c)
		{
		case 'R':
			s->str[i] = rand_val_picker(arrR, 2);
			break;
		case 'Y':
			s->str[i] = rand_val_picker(arrY, 2);
			break;
		case 'K':
			s->str[i] = rand_val_picker(arrK, 2);
			break;
		case 'M':
			s->str[i] = rand_val_picker(arrM, 2);
			break;
		case 'S':
			s->str[i] = rand_val_picker(arrS, 2);
			break;
		case 'W':
			s->str[i] = rand_val_picker(arrW, 2);
			break;
		case 'B':
			s->str[i] = rand_val_picker(arrB, 3);
			break;
		case 'D':
			s->str[i] = rand_val_picker(arrD, 3);
			break;
		case 'H':
			s->str[i] = rand_val_picker(arrH, 3);
			break;
		case 'V':
			s->str[i] = rand_val_picker(arrV, 3);
			break;
		case 'N':
			s->str[i] = rand_val_picker(arrN, 4);
			break;
		}
	}

}