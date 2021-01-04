// randlibsw.c should contain the software implementation of the random
// number generator. Like randlibhw.c, it should start by including
// randlib.h and should implement the interface described by randlib.h.
// Since the software implementation needs initialization and finalization,
// this implementation should also define an initializer and a finalizer
// function, using GCC's "__attribute__ ((constructor))" and
// "__attribute__ ((destructor))" declaration specifiers

#include "randlib.h"
#include <stdlib.h>
#include <stdio.h>


//constructor and destructor attributes
static void __attribute__((constructor)) software_rand64_init();
static void __attribute__((destructor)) software_rand64_fini();

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Initialize the software rand64 implementation.  */
static void
software_rand64_init (void)
{
  urandstream = fopen ("/dev/urandom", "r");
  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
static unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

extern unsigned long long
rand64(void) {
  return software_rand64();
}

/* Finalize the software rand64 implementation.  */
static void
software_rand64_fini (void)
{
  fclose (urandstream);
}
