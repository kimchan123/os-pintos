#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <string.h>
#include "userprog/syscall.h"
#include "lib/user/syscall.h"
int
main(int argc, char **argv){
	printf("%d ", fibonacci(atoi(argv[1])));
	printf("%d\n", max_of_four_integers(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4])));
	return EXIT_SUCCESS;
}

