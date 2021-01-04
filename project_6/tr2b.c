// Name: Joseph Picchi
// UID:  605-124-511
// Assignment 6
// tr2b.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char* argv[]) {

  // PART 1: ERROR CHECKING
  
  //argc must be 3
  if (argc < 2) {
    fprintf(stderr, "tr2b error: missing operand\n");
    exit(1);
  }
  else if (argc < 3) {
    fprintf(stderr, "tr2b error: missing operand after '%s'\n\
Two strings must be given when translating\n", argv[1]);
    exit(1);
  }
  else if (argc > 3) {
    fprintf(stderr, "tr2b error: extra operand '%s'\n", argv[3]);
    exit(1);
  }

  //check for mismatch in length between FROM and TO sets
  if (strlen(argv[1]) != strlen(argv[2])) {
    fprintf(stderr, "tr2b error: mismatch in length between FROM and TO sets\n");
    exit(1);
  }

  //check for duplicate chars in FROM set
  for (int i = 0; i < strlen(argv[1]); i++) {
    int first_char = *(argv[1]+i);

    for (int k = i+1; k < strlen(argv[1]); k++) {
      int second_char = *(argv[1]+k);

      if (first_char == second_char) {
	fprintf(stderr, "tr2b error: FROM set has duplicate byte '%c'\n", first_char);
	exit(1);
      }
    }
  }



  //PART 2: READING INPUT AND WRITING OUTPUT

  int c;

  while ((c = getchar()) != EOF) {

    //check if c matches a char in FROM set
    int index_match = -1;
    for (int i = 0; i < strlen(argv[1]); i++) {
      if (c == *(argv[1]+i)) {
	index_match = i;
      }
    }

    //if c matches something in FROM, replace it with corresponding char in TO
    if (index_match != -1) {
      c = *(argv[2]+index_match);
    }

    putchar(c);

    //check for write error
    if (ferror(stdout)) {
      fprintf(stderr, "tr2b error: error in writing to stdout");
      exit(1);
    }
  }

  //check for read error
  if (ferror(stdin)) {
    fprintf(stderr, "tr2b error: error in reading from stdin");
    exit(1);
  }
  
  return 0;
}
