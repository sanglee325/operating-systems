#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{
  int i;
  int arg_int[4] = { 0, };
  int fibo = 0, sum = 0;

  for(i = 1; i < argc; i++) {
  	if(argv[i] != NULL) {
		arg_int[i-1] = atoi(argv[i]);
	}
  }

  fibo = fibonacci(arg_int[0]);
  sum = sum_of_four_int(arg_int[0], arg_int[1], arg_int[2], arg_int[3]); 

  printf("%d %d\n", fibo, sum);

  return EXIT_SUCCESS;
}
