#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

int add(int argc, char *argv[]) {
	if (argc != 4) { 
		printf("Error: Must provide exactly two additional arguments. Usage: ./minicalc + N1 N2\n");
		exit(3);
	}

	int sum = 0;

	for (int i = 2; i < argc; i++) {
		errno = 0;
		char *endptr;

		if (strlen(argv[i]) > 9) {
			printf("Danger: Entered argument may cause overflow or underflow.\n");
			exit(4);
		}

		int num = (int) strtol(argv[i], &endptr, 10);
		
		//check for trailing characters
                if (*endptr != '\0') {
                        printf("Error: Argument '%s' is not a valid integer.\n", argv[i]);
                        exit(4);
                }

		//check integer conversion
		if (endptr == argv[i] || (errno != 0 && num == 0) || errno == ERANGE) {
			printf("Error: Invalid conversion.\n");	
			exit(4);
		}
		
		//check for underflow or overflow
		if ((num < INT_MIN + sum) || (num > INT_MAX - sum)) {
            		printf("Error: Invalid result.\n");
            		exit(4);
        	}

		sum += num;
	}
	return sum;
}


//Algorithm to find gcd of two numbers, learned in class of Math235
int find_gcd(int x, int y) {

	while (y) {
        	int temp = y;
       		y = x % y;
        	x = temp;
    	}	
	return x;
	//At a high level, the algorithm repeatedly replaces the larger number with the remainder of the division of the larger number by the smaller number, until the smaller number becomes zero. The last non-zero remainder is the gcd. This algorithm is based on the fundamental property that for any pair of numbers `a` and `b` expressed as `a = b * q + r`, gcd(a,b) = gcd(b,r).
}


int gcd(int argc, char *argv[]) {
	if (argc < 4) {
		printf("Error: Must provide at least two additional arguments. Usage: ./minicalc gcd N...\n");
		exit(3);
	}

	for (int i = 2; i < argc; i++) {
                errno = 0;
                char *endptr;

		if (strlen(argv[i]) > 9) {
                        printf("Danger: Entered argument may cause overflow or underflow.\n");
                        exit(4);
                }

                int num = (int) strtol(argv[i], &endptr, 10);

		//check for trailing characters
                if (*endptr != '\0') {
                        printf("Error: Argument '%s' is not a valid integer.\n", argv[i]);
                        exit(4);
                }

                //check integer conversion
                if (endptr == argv[i] || errno == ERANGE || (errno != 0 && num == 0)) {
                        printf("Error: Invalid conversion.\n");
                        exit(4);
                }
             
		//check if number is greater than 0
		if (num <= 0) {
			printf("Error: Inputs must be greater than zero\n");
			exit(7);
		}
	}
	
	int gcd = find_gcd(strtol(argv[2], NULL, 10), strtol(argv[3], NULL, 10));
	
	if (argc > 4) {
		for (int j = 4; j < argc; j++)
                	gcd = find_gcd(gcd, strtol(argv[j], NULL, 10));
	} //To find the gcd of three or more numbers, I repeatedly take the gcd of the first calculated gcd with the next number

	return gcd;
}


double square_root(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Error: Must provide exactly one additional argument. Usage: ./minicalc sqrt A1\n");
		exit(3);
	}

        errno = 0;
        char *endptr;
        double num = strtod(argv[2], &endptr);

	//check for trailing characters
        if (*endptr != '\0') {
                printf("Error: Argument '%s' is not a valid double.\n", argv[2]);
                exit(4);
        }       
       
	//check double conversion
        if (endptr == argv[2] || errno == ERANGE || (errno != 0 && num == 0)) {
		printf("Error: Invalid conversion.\n");
		exit(4);	
	}
	
	//check if number is nonnegative
	if (num < 0) {
		printf("Error: Input must be nonnegative\n");
		exit(5);
	}

	return sqrt(num);
}


// helper method for character comparaison
int charcmp(const void *a, const void *b) {
                return (*(char *)a - *(char *)b);
        }


int is_anagram(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Error: Must provide exactly two additional arguments. Usage: ./minicalc anagram S1 S2\n");
		exit(3);
	}

	char *s1 = argv[2];
	char *s2 = argv[3];
	
	for (int i = 0; s1[i] != '\0'; i++) {
		if (!islower(s1[i])) {
			printf("Error: Inputs must consist only of lowercase letters\n");	
			exit(6);
		}
	}

	for (int j = 0; s2[j] != '\0'; j++) {
                if (!islower(s2[j])) {
			printf("Error: Inputs must consist only of lowercase letters\n");	
			exit(6);
		}
        }

	// sort the input strings into alphabetical order
	qsort(s1, strlen(s1), sizeof(char), charcmp);
    	qsort(s2, strlen(s2), sizeof(char), charcmp);

	if (strlen(s1) == strlen(s2) && strcmp(s1, s2)==0) return 1;
	else
		return 0;
}	


int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Error: Insufficient arguments. Usage: ./minicalc OP ARGS...\n");
		exit(1);
	}

	char *operation = argv[1];

	if (strcmp(operation, "+")==0) {
		printf("%d\n", add(argc, argv));
	} else if (strcmp(operation, "gcd")==0) {
        	printf("%d\n", gcd(argc, argv));
        } else if (strcmp(operation, "sqrt")==0) {
		printf("%lf\n", square_root(argc, argv));
        } else if (strcmp(operation, "anagram")==0) {
        	if (is_anagram(argc, argv)) printf("true\n");
		else printf("false\n");
	} else {
		printf("Error: Invald operation. Allowed operations: +, gcd, sqrt, anagram\n");
		exit(2);
	}

	exit(0);
}
