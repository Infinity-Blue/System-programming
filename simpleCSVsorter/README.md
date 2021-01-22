## Simple CSV Sorter
### Description
simpleCSVsorter reads CSV file from STDIN, tokenizes data and allocates the memory dynamically to store tokenized data. SimpleCSVsorter contains struct Field_Set for defining all names and types of column headers each field can have. Once storing all data is finished, the program calls mergesort and mergesort sorts CSV file in ascending order. Then simpleCSVsorter outputs sorted CSV record.

SimpleCSVsorter - A program for reading CSV file from STDIN and outputting sorted CSV record.

Mergesort - A program for sorting data in ascending order.

A header file(simpleCSVsorter.h) contains structs which respectively serve as structures for the record and the column headings read from CSV file and prototyped mergesort functions. 

### Assumptions
1. The type of the fields adheres to the format given in the assignment
2. But, the number of the fields and the order of the fields could be changed
3. Each field could have the delimiter ‘,’ inside.
4. Each field is separated by ‘,’.
5. The number of headings/columns does not exceed 28.
6. Each line of CSV file is separated by ‘\n’.
7. Output should be same as input. (Non-trimmed input should be outputted as non- trimmed)
8. Each field can have leading and trailing whitespaces.

### How to test code
For testing purposes you can redirect STDOUT to a file by the following:
```
cat input.file | ./simpleCSVsorter -c movie_title > sortedmovies.csv
```
### How to run code

The first argument to your program will be '-c' to indicate sorting by column and the second will be the column name:
```
./simpleCSVsorter -c food
```
