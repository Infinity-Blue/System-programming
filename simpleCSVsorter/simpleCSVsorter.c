#include "simpleCSVsorter.h"

// #define	DEBUG		1
#define DEBUG0929      	1

#define	En_Able		1
#define	Dis_Able	0

int	RecordCount;		
int	DelimiterCount;
int	DelimiterFlag;		
Title_Set	title_set;	/* The struct for the Column Haeding (Name). There are totally 28 column haedings */
Data_Set **data_set;	/* The struct for the Data Record */

/*
Return the Pointer of the Token
Parameter 
1. char *str : The pointer of the string which needs to be parsing
2. int *str_prt : The parsing will be started from this postion of the string. 
*/
static char *myStrtok(char *str, int *str_ptr);
/* 
Whenever we meet the delimitor, the counter of the delimitor is inccreased by one, and
Whenever the counter of delimitor is over the 28 column headings, the counter of the record is increased by one
*/
void counter_up();		

/*
Display the seult to the Stdout
*/
void display_result();

/*
*/
int data_parsing(char *token);
int find_title_index(int number);

#ifdef DEBUG
FILE *fp_w;
#endif

int no_columns;

char *Field_Set_Name [] = 
{
   "color",
   "director_name", 
   "num_critic_for_reviews",
   "duration", 
   "director_facebook_likes", 
   "actor_3_facebook_likes", 
   "actor_2_name", 
   "actor_1_facebook_likes", 
   "gross", 
   "genres", 
   "actor_1_name", 
   "movie_title", 
   "num_voted_users", 
   "cast_total_facebook_likes", 
   "actor_3_name", 
   "facenumber_in_poster",
   "plot_keywords", 
   "movie_imdb_link",
   "num_user_for_reviews",
   "language", 
   "country", 
   "content_rating", 
   "budget", 
   "title_year", 
   "actor_2_facebook_likes", 
   "imdb_score", 
   "aspect_ratio", 
   "movie_facebook_likes"
};

int Field_Set_Type [] =
{
   STRING,      // color
   STRING,      // director_name
   INTEGER,   // num_critic_for_reviews
   INTEGER,   // duration
   INTEGER,    // director_facebook_like
   INTEGER,   // actor_3_facebook_likes 
   STRING,      // actor_2_name 
   INTEGER,   // actor_1_facebook_likes
   FLOAT,      // gross
   STRING,      // genres
   STRING,      // actor_1_name
   STRING,      // movie_title
   INTEGER,   // num_voted_users
   INTEGER,   // cast_total_facebook_likes
   STRING,      // actor_3_name
   INTEGER,   // facenumber_in_poster
   STRING,      // facenumber_in_poster
   STRING,      // move_imdb_link
   INTEGER,   // num_user_for_reviews
   STRING,      // language
   STRING,      // country
   STRING,      // content_rating
   FLOAT,      // budget
   INTEGER,   // title_year
   INTEGER,   // actor_2_facebook_likes
   FLOAT,      // imdb_score
   FLOAT,      // aspect_ratio
   INTEGER      // movie_facebook_likes
};

int main(argc, argv)
int	argc;
char *argv[];
{

	FILE *fp;

	int Column_No_For_Sort;
	
	char *token;
	int token_len;	
	char buffer[BUF_SIZE];		
	int buf_ptr;

	fp = stdin;
	no_columns = No_Columns;			
	
	if (argc != 3) 
	{
		fprintf(stderr, "Arguments are not correct \n");
		//system("cat Asst0Doc.pdf");
		return(-1);
	}
        
    if (strcmp(argv[1], "-c"))
	{
		fprintf(stderr, "Arguments are not correct \n");
		//system("cat Asst0Doc.pdf");
		return(-1);
	}   
	
    RecordCount = 0;
    DelimiterCount = 0;

    DelimiterFlag = En_Able;
    
#ifdef DEBUG
	fp = fopen("movie_metadata (1).csv","r");
	fp_w = fopen("sortedmovies (1).csv","w");
#endif	
	
    while(!feof(fp))
    {
    	memset(buffer, 0, BUF_SIZE);
		if (fgets(buffer, BUF_SIZE, fp) == NULL)
		{
    			break;
		}		
		buf_ptr = 0;
#ifdef	DEBUG
	printf("The string come from the Stdin is = %s \n", buffer);
#endif
		token = myStrtok(buffer, &buf_ptr);
		while (token != NULL)				/* In case that a token is found */
		{
			if (RecordCount == 0)			/* In case that it is the first record. It is the column headingss */
			{
				token_len = strlen(token);

				title_set.column_headings[DelimiterCount % no_columns] = (char *) malloc (token_len + 1);	/* Malloc allocation for the column heading */
   				if (title_set.column_headings[DelimiterCount % no_columns] == NULL)							/* In case that there is an error in the malloc */
   				{	
   					/* Display malloc error msg in the Stderr */
   					fprintf(stderr, "malloc error in getting the memory for the column heading \n");
   					return (-1);
   				}
				if (token_len != 0)			/* In case the column haeding is NOT empty */				  	
    				strncpy (title_set.column_headings[DelimiterCount % no_columns], token, token_len);		/* Copy the token to the structure for the column heading */
   				*(title_set.column_headings[DelimiterCount % no_columns] + token_len) = 0;
#ifdef	DEBUG
				printf( "Column No is %d, Column is %s, token_len is %d\n", DelimiterCount % no_columns, title_set.column_headings[DelimiterCount % no_columns], token_len);  
#endif  
			}
			else										/* In case of the data */
			{
				if (DelimiterCount == no_columns)		/* In case that it is the first row for the data, it is necessary for us to check whether if the argv[2] is one of the column headings */
				{
					for (Column_No_For_Sort = 0 ; Column_No_For_Sort < no_columns; Column_No_For_Sort++)
					{
#ifdef	DEBUG
						printf( "no is %d, argv[2] is %s, Column is %s\n", Column_No_For_Sort, argv[2], title_set.column_headings[Column_No_For_Sort] );  
#endif  
						if (!strcmp(argv[2], title_set.column_headings[Column_No_For_Sort]))
							break;
					}
 
					if (Column_No_For_Sort == no_columns)				/* In case that the argv[2] is NOT one of the column headings */
					{	
					   	fprintf(stderr, "The column name %s is not existed \n", argv[2]);
						//system("cat Asst0Doc.pdf");
						return(-1);
					}	   
				}
  				
				if (data_parsing(token) < 0)
				{
					return(-1);
				}

			}
			
			counter_up();					/* Increase the number of counters */
			
			token = myStrtok(buffer, &buf_ptr);
		}		
	}

	if (RecordCount > 1)
	{
		if (mergesort(data_set, Column_No_For_Sort, 0, RecordCount - 2) < 0)	/* ø¯∑°¥¬ RecoedCount - 1 ¿ÃæÓæﬂ «œ¥¬µ•...RecordCount∞° «œ≥™ ¥ı √ﬂ∞°µ» ªÛ≈¬∂Û.... -2∏¶ «ÿæﬂ «‘ */ 
			return(-1);
	}
			
	display_result();

    return 0;
}

/*
Return the Pointer of the Token
Parameter 
1. char *str : The pointer of the string which needs to be parsing
2. int *str_prt : The parsing will be started from this postion of the string. 
*/
static char *myStrtok (char *str, int *str_ptr)
{
	int i, j;
	static int buf_token_ptr = 0;
	int	str_length;
	str_length = strlen (str);
	static char buffer_token[BUF_SIZE];

#ifdef	DEBUG0929
	if (buf_token_ptr == 0)
	memset(buffer_token, 0, BUF_SIZE);
	
	for (i = 0; i < BUF_SIZE; i++)
	{
		j = *str_ptr + i;
		if (j > str_length)
		{
			break;
		}
		
		if (str[j] == '"')					/* In case we meet the Special Quote Character */
		{
			if (DelimiterFlag == En_Able)
				DelimiterFlag = Dis_Able;
			else
				DelimiterFlag = En_Able;
		}
		if (str[j] == ',' || str[j] == '\n')			/* In case that we meet the delimitor */
		{
			if (DelimiterFlag == Dis_Able && str[j] == ',')				/* In case that the delimitor is included in the data. It means the the delimitor is just data itself not the delimitor */
			{
				buffer_token[buf_token_ptr++] = str[j];
			}
			else
			{
				/* In case that it is the delimitor or the new line character */	
				buffer_token[buf_token_ptr] = 0;
#ifdef DEBUG0929
				buf_token_ptr = 0;
				if (RecordCount == 0 && str[j] == '\n')	// when meet the first new line, the number of columns (Fields) is fixed
				{
					no_columns = DelimiterCount+1;
				}
#endif
				*str_ptr = ++j;							/* Set the current postion of the string to the variable "str_ptr" */
				return (buffer_token);					/* Return the pointer of the token */
			}						
		}
		else
		{
			buffer_token[buf_token_ptr++] = str[j];	
		}
	}
#ifdef DEBUG0929
	if (buf_token_ptr > 0)
		buf_token_ptr--;
#endif
	return (NULL); 										/* in case that the token is not found */
}


void counter_up ()
{
    DelimiterCount++;
    RecordCount = DelimiterCount / no_columns;
#ifdef	DEBUG
	printf( "DelimiterCount is %d, RecordCount is %d\n", DelimiterCount, RecordCount);
#endif  
}


int data_parsing(char *token)
{
	int i,j, tok_len;
	
	if ((DelimiterCount % no_columns) == 0)
	{
		if (RecordCount == 1)
			data_set = (Data_Set **) malloc (sizeof(Data_Set) * RecordCount);			/* In the first time, malloc */
		else
		{
#ifdef	DEBUG
			printf( "Realloc RecordCount is %d, Address is %x, Memory Size is %d %d\n", RecordCount, data_set, sizeof(Data_Set) * RecordCount, sizeof(Data_Set));  
#endif 
			data_set = (Data_Set **) realloc ((void *)data_set, sizeof(Data_Set) * RecordCount);	/* Since the second times, realloc */
		}
		if (data_set == NULL)															/* In case that there is an error in the malloc */
   		{	
   			/* Display malloc error msg in the Stderr */
   			fprintf(stderr, "malloc and/or realloc error in getting the memory for the data record \n");
   			return (-1);
   		}
   		
   		data_set[RecordCount-1] = (Data_Set *) malloc (sizeof (Data_Set));
   		if (data_set[RecordCount-1] == NULL)	/* In case that there is an error in the malloc */
   		{	
   			/* Display malloc error msg in the Stderr */
   			fprintf(stderr, "malloc error in getting the memory for the data record \n");
   			return (-1);
   		}
   		memset(data_set[RecordCount-1], 0, sizeof (Data_Set));
#ifdef	DEBUG
	printf( "address of data set is %x\n", data_set[RecordCount-1]);  
#endif
	}

	i = DelimiterCount % no_columns;	
#ifdef	DEBUG0929		
	j = find_title_index(DelimiterCount % no_columns);
	if (j < no_columns)
	{
                 data_set[RecordCount-1]->type[i] = Field_Set_Type[j];

	}
#endif	
	tok_len = strlen(token);
#ifdef	DEBUG
	printf( "token length is %d, record no is %d, field no is %d\n", tok_len, RecordCount, i);  
#endif 

	data_set[RecordCount-1]->field_ptr[i] = (char *) malloc (tok_len + 1);
   	if (data_set[RecordCount-1]->field_ptr[i] == NULL)							
   	{
   		fprintf(stderr, "malloc error in getting the memory for the column no %d \n", i);
		return (-1);
	}
	strcpy(data_set[RecordCount-1]->field_ptr[i], token);
	*(data_set[RecordCount-1]->field_ptr[i] + tok_len)= 0;

	return (0);
}

int find_title_index(int number)
{
	int i;	
	
	for (i = 0; i < no_columns; i++)
	{
             if (!strcmp(Field_Set_Name[i], title_set.column_headings[number]))	

			break;
	}
	return (i);
}


void display_result()
{
	int i, j, k;
		
	for (i = 0; i < DelimiterCount; i++)
	{
		j = i / no_columns;
		k = i % no_columns;
		if (j == 0)
		{
			fprintf(fp_w, "%s", title_set.column_headings[i % no_columns]);
			printf("%s", title_set.column_headings[i % no_columns]);
		}
		
		else
		{
			fprintf(fp_w, "%s", (char *)data_set[j-1]->field_ptr[k]);
			printf("%s", (char *)data_set[j-1]->field_ptr[k]);
		}
		
		if (k == (no_columns - 1))
		{
			fprintf(fp_w, "\n");
			printf("\n");
		}
		
		else
		{
			fprintf(fp_w, ",");
			printf(",");
		}
	}
}
