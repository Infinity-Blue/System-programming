#include "simpleCSVsorter.h"
/*
Trim leading & trailing whitespace
*/

int mergesort(Data_Set **data_set, int column_no_for_sort, int start, int end)
{
    int mid, ret;
    
    if(start < end)
    {
        mid=(start + end) / 2;
        ret = mergesort(data_set, column_no_for_sort, start, mid);       	
        if (ret < 0)
        {
		return (-1);
	}
  		
        ret = mergesort(data_set, column_no_for_sort, mid + 1, end);    		
        if (ret < 0)
        {
        	return (-1);
	}

        ret = merge(data_set, column_no_for_sort, start, mid, end);    		// merging of two sorted sub-arrays
	        
        if (ret < 0)
        {
        	return (-1);
	}
    }
    return (0);
}
 
int merge(Data_Set **data_set, int column_no_for_sort, int start, int mid, int end)
{

    Data_Set **temp_data_set;   // array used for merging
    char *str1;
    char *str2;
    int int_value1, int_value2;
    float float_value1, float_value2;
    int i, p, q;
    int tmp_index, tmp_recordcount;
    p = start;    				// beginning of the first list
    q = mid + 1;   				// beginning of the second list
    tmp_index = 0;
    tmp_recordcount = end - start + 1;
    
    temp_data_set = (Data_Set **) malloc (sizeof(Data_Set) * tmp_recordcount);
    
	if (temp_data_set == NULL)															// In case of malloc error
   	{	
   		fprintf(stderr, "malloc error in getting the memory for the data_set in the merge sorting \n");	// Display malloc error msg in the Stderr
   		return (-1);
   	}

	if (temp_memory_allocation(temp_data_set, tmp_recordcount) < 0)
	{
		fprintf(stderr, "malloc error in getting the temporary memory for the merge sorting \n");	// Display malloc error msg in the Stderr
   		return (-1);
	}

    for (i = tmp_index; i < end - start; i++)
    {
    	while(p <= mid && q <= end)   	
    	{
    		str1 = (char *) malloc (strlen(data_set[p]->field_ptr[column_no_for_sort]) + 1);
    		str2 = (char *) malloc (strlen(data_set[q]->field_ptr[column_no_for_sort]) + 1);
    		strcpy(str1, data_set[p]->field_ptr[column_no_for_sort]);
    		strcpy(str2, data_set[q]->field_ptr[column_no_for_sort]);
    		*(str1 + strlen(data_set[p]->field_ptr[column_no_for_sort])) = 0;
    		*(str2 + strlen(data_set[q]->field_ptr[column_no_for_sort])) = 0;
    		trimString(str1);
    		trimString(str2);

			if (data_set[p]->type[column_no_for_sort] == 1 && data_set[q]->type[column_no_for_sort] == 1 )			// int type
			{	
				int_value1 = atoi(str1);
				int_value2 = atoi(str2);
				if(int_value1 < int_value2)
				{	
					*temp_data_set[tmp_index++] = *data_set[p++];
				}	
				else
				{
					*temp_data_set[tmp_index++] = *data_set[q++];
				}		
			}
			else if (data_set[p]->type[column_no_for_sort] == 2 && data_set[q]->type[column_no_for_sort] == 2) 		// float type
			{	
				float_value1 = atof(str1);
				float_value2 = atof(str2);
				if(float_value1 < float_value2)
				{
					*temp_data_set[tmp_index++] = *data_set[p++];
				}
				else
				{
					*temp_data_set[tmp_index++] = *data_set[q++];
				}			
			}
			else																									// char type
			{
				if (strcmp(str1, str2) < 0)
				{
					*temp_data_set[tmp_index++] = *data_set[p++];
				}
				else
				{
					*temp_data_set[tmp_index++] = *data_set[q++];
				}	
			}
     	 }
   
	    if (p > mid)
	    {
	    	while(q <= end)    			//copy remaining elements of the first list
	    	{
		    	*temp_data_set[tmp_index++] = *data_set[q++];
		}	    
	    }
	    else
		{
			while(p <= mid)    			//copy remaining elements of the second list
			{
			    *temp_data_set[tmp_index++] = *data_set[p++];
			}
		}
    }
	
    //Transfer elements from temp_data_set[] back to data_set[]
    for(i = start; i <= end; i++)
    {
      *data_set[i] = *temp_data_set[i - start];
    }

	temp_memory_deallocation(temp_data_set, tmp_recordcount);
	free(temp_data_set);    
	return(0);
}


void trimString(char* word)
{
    int l;
    int n;
    const char* word_begins;
    const char* word_ends;

    l = strlen (word);
    n = l;
    word_begins = word;
    while (isspace (*word_begins)) {
        word_begins++;
        n--;
    }
    word_ends = word + l - 1;
    while (isspace (*word_ends)) {
        word_ends--;
        n--;
    }
    if (n != l) 
	{
		int i;
        for (i = 0; i < n; i++) {
            word[i] = word_begins[i];
        }
        word[n] = '\0';
    } 
}


int temp_memory_allocation(Data_Set **temp_data_set, int tmp_recordcount)
{
	int i;
	
	for (i = 0; i < tmp_recordcount; i++)
	{
		temp_data_set[i] = (Data_Set *) malloc (sizeof (Data_Set));
   		if (temp_data_set[i] == NULL)	/* In case that there is an error in the malloc */
   		{	
   			return (-1);
   		}
   		memset(temp_data_set[i], 0, sizeof (Data_Set));
   }
	
	return(0);
}


void temp_memory_deallocation(Data_Set **temp_data_set, int tmp_recordcount)
{
	int i;
	
	for (i = 0; i < tmp_recordcount; i++)
	{
   		free (temp_data_set[i]);	
	}
}
