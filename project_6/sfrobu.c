//Name: Joseph Picchi

//Assignment 5
//sfrob.c


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#define SSIZE_MAX 200000000

//global var
int f_passed = 0;

//==================================================================
int stdinIntoBuffer(char** buffer) {

  //obtain file properties
  struct stat file_info;
  int fstat_return = fstat(0, &file_info);

  //check for fstat error
  if (fstat_return < 0) {
    perror("fstat error");
    exit(1);
  }

  //check for empty file
  if (S_ISREG(file_info.st_mode) == 1 && file_info.st_size == 0) {
    return 0;
  }
  
  //determine buffer size
  int buffer_size = 0;
  if (S_ISREG(file_info.st_mode) == 1) {
    buffer_size = file_info.st_size + 10;
  }
  else {
    buffer_size = 8192;
  }

  //allocate a buffer
  *buffer = (char*)malloc(buffer_size);

  //check for malloc errors
  if (*buffer == NULL) {
    fprintf(stderr, "sfrobu: malloc error in 'stdinIntoBuffer'");
    exit(1);
  }

  //prepare for stdin read loop
  ssize_t read_return = 1;
  int nbytes_in_buffer = 0;
  int buffer_capacity = buffer_size - nbytes_in_buffer;
  


  while (read_return > 0) {

    //double the size if buffer is more than half full
    if (nbytes_in_buffer > buffer_size/2) {
      buffer_size *= 2;
      *buffer = (char*)realloc(*buffer, buffer_size);
      buffer_capacity = buffer_size - nbytes_in_buffer;

      //check for realloc failure
      if (*buffer == NULL) {
	fprintf(stderr, "sfrobu: realloc failure in 'stdinIntoBuffer'");
	exit(1);
      }
    }
    
    //read into buffer
    int read_bytes = buffer_capacity-1;
    if (buffer_capacity > SSIZE_MAX) {
      read_bytes = SSIZE_MAX;
    }
    read_return = read(0, *buffer+nbytes_in_buffer, read_bytes);
    nbytes_in_buffer += read_return;
    buffer_capacity = buffer_size - nbytes_in_buffer;
  } 

  //check for read error
  if (read_return < 0) {
    fprintf(stderr, "sfrobu: read error for regular file type");
    exit(1);
  }

  return nbytes_in_buffer;
}



//=========================================================
void deallocate(char** array2d_ptr, int array2d_length, char* buffer,
		char* buffer2) {

  //free all elements of array2d
  for (int i = 0; i < array2d_length; i++) {
    free( *(array2d_ptr + i) );
    *(array2d_ptr + i) = NULL;
  }

  free(array2d_ptr);
  array2d_ptr = NULL;
  free(buffer);
  buffer = NULL;
  free(buffer2);
  buffer2 = NULL;
}

//=============================================================
void readWriteError() {

  //print error message to stderr
  fprintf(stderr, "error in reading from stdin or writing to stdout");

}

//================================================================
void allocationError() {

  //print error message to stderr
  fprintf(stderr, "sfrobu: error in allocating memory");
  
}

//==================================================================

//return 1 if a > b, 0 if a == b, -1 if a < b
//if one is prefix of other, then consider prefix to be less than longer
int frobcmp(char const* a, char const* b) {
  
  char const* tempA = a;
  char const* tempB = b;
  int decryptedA = 0;
  int decryptedB = 0;

  //compare char by char until you reach a space
  while (*tempA != 32 && *tempB != 32) {

    decryptedA = *tempA ^ 42;
    decryptedB = *tempB ^ 42;

    //use toupper() if needed
    if (decryptedA > 96 && decryptedA < 123 && f_passed == 1) {
      decryptedA = toupper(decryptedA);
    }
    if (decryptedB > 96 && decryptedB < 123 && f_passed == 1) {
      decryptedB = toupper(decryptedB);
    }

    //check if one string greater than other
    if ( decryptedA > decryptedB ) {
      return 1;
    }
    else if ( decryptedA < decryptedB ) {
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


//===============================================================
int compare(const void* a, const void* b) {
  return frobcmp(*((char const**)a), *((char const**)b));
}


//================================================================
int bufferIntoArray(char*** array_ptr, int* length_ptr,
		    int* empty_ptr, char** buffer, int* nbytes_in_buffer) {

  //variables for loop
  char current_char = 32;
  int byte_index = 0;
  char last_char = 32;
  int column_length = 1;
  char* ptr_char;

  while (byte_index < *nbytes_in_buffer) {

    //get next char and update last one
    last_char = current_char;
    current_char = *(*buffer+byte_index);

    //create a new row and column if last_char was a space
    if (*empty_ptr == 0 && last_char == 32) {

      //add a new row
      *length_ptr += 1;
      *array_ptr = (char**)realloc(*array_ptr, *length_ptr * sizeof(char*));

      //check for allocation failure
      if (*array_ptr == NULL) {
	allocationError();
	exit(1);
      }

      //add a new column
      *(*array_ptr + (*length_ptr - 1)) = (char*)malloc(sizeof(char));
      column_length = 1;

      //check for allocation failure
      if (*(*array_ptr + (*length_ptr - 1)) == NULL) {
	allocationError();
	exit(1);
      }
      
    }
    //create a new column if last_char was not a space
    else if (*empty_ptr == 0) {

      //create a new column for the current row
      column_length++;
      *(*array_ptr + (*length_ptr - 1)) = (char*)realloc(*(*array_ptr
								 +(*length_ptr - 1)),
							       column_length
							       * sizeof(char));

      //check for allocation failure
      if ( *(*array_ptr + (*length_ptr - 1)) == NULL) {
	allocationError();
	exit(1);
      }

    }

    //put the char in the newly created spot
    ptr_char = *(*array_ptr + (*length_ptr - 1));
    ptr_char += column_length - 1;
    *ptr_char = current_char;
    
    //update vars for next iteration
    byte_index += 1;
    *empty_ptr = 0;

  } //END OF WHILE


  //add a space at the end if current_char is not a space
  if (current_char != 32) {

    //add a column
    column_length++;
    *(*array_ptr + (*length_ptr - 1)) = (char*)realloc(*(*array_ptr
							       + (*length_ptr - 1)),
							     column_length
							     * sizeof(char));

    //check for allocation error
    if ( *(*array_ptr + (*length_ptr - 1)) == NULL) {
      allocationError();
      exit(1);
    }

    //put a space in the newly created spot
    ptr_char = *(*array_ptr + (*length_ptr - 1));
    ptr_char += column_length - 1;
    *ptr_char = 32;

    *nbytes_in_buffer += 1;

  }

  return 1;
}


//====================================================================
int main(int argc, char* argv[]) {

  //check for -f flag
  if (argc > 2) {
    fprintf(stderr, "sfrobu: too many arguments passed\n");
    exit(1);
  }
  else if (argc == 2) {
    if (*argv[1] == 45 && *(argv[1]+1) == 102) {
      f_passed = 1;
    }
    else {
      fprintf(stderr, "sfrobu: invalid argument passed\n");
      exit(1);
    }
  }
  
  //read stdin into buffer
  char* buffer = NULL;
  int nbytes_in_buffer = stdinIntoBuffer(&buffer);

  //check for empty file
  if (nbytes_in_buffer == 0) {
    return 0;
  }
  
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
    exit(1);
  }

  //add one column
  *array2d.array = (char*)malloc(sizeof(char));

  //check for allocation failure
  if (*array2d.array == NULL) {
    allocationError();
    exit(1);
  }
  
  array2d.empty = 1;

  //PART 2: read input into array
  int return_status = bufferIntoArray(&array2d.array, &array2d.length,
				      &array2d.empty, &buffer, &nbytes_in_buffer);

  //exit with success if the array is empty (ie nothing from stdin)
  if (return_status == 0) {
    return 0;
  }



  //PART 3: SORT THE ARRAY
  qsort((void*)array2d.array, array2d.length, sizeof(char*),  compare);

  

  //PART 4: PRINTING

  char* ptr_current_char = NULL;
  char* buffer2 = (char*)malloc(nbytes_in_buffer+10);
  int buffer2_index = 0;

  //check for malloc error
  if (buffer2 == NULL) {
    allocationError();
    exit(1);
  }
  
  //cycle through the rows
  for (int i = 0; i < array2d.length; i++) {

    ptr_current_char = *(array2d.array + i);
    
    //print entire row until you reach a space
    while (*ptr_current_char != 32) {
      *(buffer2+buffer2_index) = *ptr_current_char;
      ptr_current_char++;
      buffer2_index++;
    }

    //print space at end of line
    *(buffer2+buffer2_index) = 32;
    buffer2_index++;
  }

  //write buffer to output
  ssize_t write_return = 0;
  int nbytes_written = 0;
  int bytes_to_write = 0;
  while(nbytes_written < nbytes_in_buffer) {

    //determine number of bytes to write
    bytes_to_write = nbytes_in_buffer-nbytes_written;
    if (nbytes_in_buffer > SSIZE_MAX) {
      bytes_to_write = SSIZE_MAX;
    }

    //write to stdout
    write_return = write(1, buffer2, bytes_to_write);

    //check for write error
    if (write_return < 0) {
      fprintf(stderr, "sfrobu: write error");
      exit(1);
    }

    nbytes_written += write_return;
  }
  
  
  //PART 5: FREEING MEMORY
  deallocate(array2d.array, array2d.length, buffer, buffer2);

  //exit successfully
  return 0;
}
