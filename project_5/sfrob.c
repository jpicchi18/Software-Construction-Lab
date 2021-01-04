//Name: Joseph Picchi
//UID:  605-124-511
//Assignment 5
//sfrob.c


#include <stdlib.h>
#include <stdio.h>


//function declarations
void deallocate(char** array2d_ptr, int array2d_length);
void readWriteError();
void allocationError();
int frobcmp(char const* a, char const* b);
int compare(const void* a, const void* b);



int main() {

  //PART 1: ALLOCATING MEMORY

  struct allInput {
    char** array;
    int length;
    int empty;
  } array2d;
  
  //create an array of one row
  array2d.array = (char**)malloc(sizeof(char*));
  array2d.length = 1;
  
  //check for allocation failure
  if (array2d.array == NULL) {
    allocationError();
    return 1;
  }

  //add one column
  *array2d.array = (char*)malloc(sizeof(char));

  //check for allocation failure
  if (*array2d.array == NULL) {
    allocationError();
    return 1;
  }
  
  array2d.empty = 1;



  //PART 2: READING INPUT INTO ARRAY

  //variables for loop
  char current_char = getc(stdin);
  char last_char = 32;
  int column_length = 1;
  char* ptr_char;

  //check for read error
  if (ferror(stdin)) {
    readWriteError();
    return 1;
  }

  while (current_char != EOF) {

    //create a new row and column if last_char was a space
    if (array2d.empty == 0 && last_char == 32) {

      //add a new row
      array2d.length += 1;
      array2d.array = (char**)realloc(array2d.array, array2d.length * sizeof(char*));

      //check for allocation failure
      if (array2d.array == NULL) {
	allocationError();
	return 1;
      }

      //add a new column
      *(array2d.array + (array2d.length - 1)) = (char*)malloc(sizeof(char));
      column_length = 1;

      //check for allocation failure
      if (*(array2d.array + (array2d.length - 1)) == NULL) {
	allocationError();
	return 1;
      }
      
    }
    //create a new column if last_char was not a space
    else if (array2d.empty == 0) {

      //create a new column for the current row
      column_length++;
      *(array2d.array + (array2d.length - 1)) = (char*)realloc(*(array2d.array
								 +(array2d.length - 1)),
							       column_length
							       * sizeof(char));

      //check for allocation failure
      if ( *(array2d.array + (array2d.length - 1)) == NULL) {
	allocationError();
	return 1;
      }

    }

    //put the char in the newly created spot
    ptr_char = *(array2d.array + (array2d.length - 1));
    ptr_char += column_length - 1;
    *ptr_char = current_char;
    
    //update vars for next iteration
    last_char = current_char;
    current_char = getc(stdin);
    array2d.empty = 0;

    //check for read error
    if (ferror(stdin)) {
      readWriteError();
      return 1;
    }
    
  }

  //nothing to do if we added no elements
  if (array2d.empty == 1) {
    deallocate(array2d.array, array2d.length);
    return 0;
  }


  //add a space at the end if last_char is not a space
  if (last_char != 32) {

    //add a column
    column_length++;
    *(array2d.array + (array2d.length - 1)) = (char*)realloc(*(array2d.array
							       + (array2d.length - 1)),
							     column_length
							     * sizeof(char));

    //check for allocation error
    if ( *(array2d.array + (array2d.length - 1)) == NULL) {
      allocationError();
      return 1;
    }

    //put a space in the newly created spot
    ptr_char = *(array2d.array + (array2d.length - 1));
    ptr_char += column_length - 1;
    *ptr_char = 32;

  }



  //PART 3: SORT THE ARRAY
  qsort((void*)array2d.array, array2d.length, sizeof(char*),  compare);

  

  //PART 4: PRINTING

  char* ptr_current_char = NULL;
  
  //cycle through the rows
  for (int i = 0; i < array2d.length; i++) {

    ptr_current_char = *(array2d.array + i);
    
    //print entire row until you reach a space
    while (*ptr_current_char != 32) {

      putchar(*ptr_current_char);

      //check for write error
      if (ferror(stdin)) {
	readWriteError();
	return 1;
      }

      ptr_current_char++;
    }

    //print space at end of line
    putchar(32);

    //check for write error
    if (ferror(stdout)) {
      readWriteError();
      return 1;
    }

  }
  
  
  //PART 5: FREEING MEMORY
  
  deallocate(array2d.array, array2d.length);

  //exit successfully
  return 0;

}




void deallocate(char** array2d_ptr, int array2d_length) {

  //free all elements of array2d
  for (int i = 0; i < array2d_length; i++) {
    free( *(array2d_ptr + i) );
    *(array2d_ptr + i) = NULL;
  }

  free(array2d_ptr);
  array2d_ptr = NULL;
  
}


void readWriteError() {

  //print error message to stderr
  fprintf(stderr, "error in reading from stdin or writing to stdout");

}


void allocationError() {

  //print error message to stderr
  fprintf(stderr, "error in allocating memory");
  
}


//return 1 if a > b, 0 if a == b, -1 if a < b
//if one is prefix of other, then consider prefix to be less than longer
int frobcmp(char const* a, char const* b) {
  
  char const* tempA = a;
  char const* tempB = b;

  //compare char by char until you reach a space
  while (*tempA != 32 && *tempB != 32) {

    //check if one string greater than other
    if ( (*tempA ^ 42) > (*tempB ^ 42) ) {
      return 1;
    }
    else if ( (*tempA ^ 42) < (*tempB ^ 42) ) {
      return -1;
    }

    tempA++;
    tempB++;

  }

  //if we got here, then one (or both) of tempA/tempB points at a space
  if (*tempA == 32 && *tempB == 32) {
    return 0;
  }
  else if (*tempA == 32) {
    return -1;
  }
  else if (*tempB == 32) {
    return 1;
  }
  else {
    return 2;  //no other options possible --> error!
  }
  
}



int compare(const void* a, const void* b) {
  return frobcmp(*((char const**)a), *((char const**)b));
}
