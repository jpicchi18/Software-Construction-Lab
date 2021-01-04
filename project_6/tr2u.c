// Name: Joseph Picchi
// UID:  605-124-511
// Assignment 6
// tr2u.c

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>



int main(int argc, char* argv[]) {

  // PART 1: ERROR CHECKING
  
  //argc must be 2
  if (argc < 2) {
    fprintf(stderr, "tr2u error: missing operand\n");
    exit(1);
  }
  else if (argc < 3) {
    fprintf(stderr, "tr2u error: missing operand after '%s'\n\
Two strings must be given when translating\n", argv[1]);
    exit(1);
  }
  else if (argc > 3) {
    fprintf(stderr, "tr2u error: extra operand '%s'\n", argv[3]);
    exit(1);
  }

  //check for mismatch in length between FROM and TO sets
  if (strlen(argv[1]) != strlen(argv[2])) {
    fprintf(stderr, "tr2u error: mismatch in length between FROM and TO sets\n");
    exit(1);
  }

  //check for duplicate chars in FROM set
  for (int i = 0; i < strlen(argv[1]); i++) {
    int first_char = *(argv[1]+i);

    for (int k = i+1; k < strlen(argv[1]); k++) {
      int second_char = *(argv[1]+k);

      if (first_char == second_char) {
	fprintf(stderr, "tr2u error: FROM set has duplicate byte '%c'\n", first_char);
	exit(1);
      }
    }
  }



  //PART 2: READING INPUT AND WRITING OUTPUT

  int buffer[1] = {0};
  ssize_t read_error = 0;
  
  while ((read_error = read(0, buffer, 1)) > 0) {

    //check if c matches a char in FROM set
    int index_match = -1;
    for (int i = 0; i < strlen(argv[1]); i++) {
      if (buffer[0] == *(argv[1]+i)) {
	index_match = i;
      }
    }

    //if c matches something in FROM, replace it with corresponding char in TO
    if (index_match != -1) {
      buffer[0] = *(argv[2]+index_match);
    }

    ssize_t write_error = write(1, buffer, 1);

    //check for write error
    if (write_error < 0) {
      fprintf(stderr, "tr2u error: error in writing to stdout");
      exit(1);
    }
  }

  //check for read error
  if (read_error < 0) {
    fprintf(stderr, "tr2u error: error in reading from stdin");
    exit(1);
  }

  return 0;
}
