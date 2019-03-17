/*
*	Define structures and function prototypes for your sorter
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define	BUF_SIZE	1028

//Suggestion: define a struct that mirrors a record (row) of the data set

#define	No_Columns	28
#define	STRING		0
#define	INTEGER		1
#define	FLOAT		2

/*
1. struct for the data
2. whenever there is a new record (row), allocate the memory dynamically
3. In case of the first record (row), use "malloc"
   in the other case, use "realloc" in order to resize the memory 
*/
typedef struct 
{
	int	type[No_Columns];
	char *field_ptr[No_Columns];
} Data_Set;

/*
1. struct for the column header
*/
typedef	struct
{
	char *column_headings[No_Columns];
} Title_Set;


//Suggestion: prototype a mergesort function

int mergesort(Data_Set **data_set, int column_no_for_sort, int start, int end);
int merge(Data_Set **data_set, int column_no_for_sort, int start, int mid, int end);

int temp_memory_allocation(Data_Set **temp_data_set, int tmp_recordcount);
void temp_memory_deallocation(Data_Set **temp_data_set, int tmp_recordcount);
/*
Trim leading & trailing whitespace
*/
void trimString(char* str);
