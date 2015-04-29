#include <stdio.h>
#include <string.h>
#define MAXLEN 1000

void chars_to_ints(char s[],int x[]);
void ints_to_chars(int x[],char s[],int max);
void reverse(char s[]);
void reduce_to_digits(int x[], int n);
int largeint_length(int x[], int max);
int largeint_multiply(int x[],int y[],int z[],int max);



// code to multiply two very large integers using integer arrays
// to represent large ints, up to 999 digits.
//
// The convention is that a large integer X is defined as
// the sum from 0 to n of i[n]*10^n
//
// Thus the digits in the array are represented in REVERSE order.


/* executable takes two arguments from the command line which must be integer strings */

int main(int argc, char * argv[])
{
	
	int x[MAXLEN] = {0};
	int y[MAXLEN] = {0};
	int z[MAXLEN] = {0};
	
	/* reverse the string to put it in std form */
	reverse(argv[1]);
	reverse(argv[2]);
	
	/* convert string to array of ints */
	chars_to_ints(argv[1],x);
	chars_to_ints(argv[2],y);
	
	int m = largeint_length(x, MAXLEN);
	int n = largeint_length(y, MAXLEN);
	
	largeint_multiply(x,y,z,MAXLEN);
		
	char s[MAXLEN+1];
	ints_to_chars(z,s,MAXLEN);
	reverse(s);
	printf("%s\n", s);
	
		
	return 0;
}

/* Functions follow */

void reverse(char s[])
{
	int len = strlen(s);
	for (int i = 0; i < len/2; i++) {
		char temp = s[i];
		s[i] = s[len-1 - i];
		s[len-1-i] = temp;
	}
}

/* find length of integer array -- assumes unwanted elements
 have been initialized to zero */
int largeint_length(int x[], int max)
{
	int i = max;
	while (x[i-1] == 0 && i > 1)
		i--;
	return i;
}

void chars_to_ints(char s[],int x[])
{
	int m = strlen(s);

	for (int i = 0; i < m; i++)
		x[i] = (s[i] - '0');
}

void ints_to_chars(int x[],char s[],int max)
{
	int m = largeint_length(x,max);
	
	for (int i = 0; i < m; i++)
		s[i] = (x[i] + '0');
	
	s[m] = '\0';
}


/* take a number represented as an array of ints of maximum length n, some of which may be larger than 9, and convert it to digits ranging from 0 to 9. */
void reduce_to_digits(int x[], int max)
{
	for(int i = 0; i < max; ++i) {
		x[i+1] += x[i]/10;
		x[i] %= 10;
	}
}

int largeint_multiply(int x[], int y[], int z[], int max)
{
	for(int i=0; i< max; i++)
		z[i] = 0;	/* ensure that output has been initialized */
	
	
	int m = largeint_length(x, max);
	int n = largeint_length(y, max);
	
	for(int i = 0; i < m; i++) {
		for(int j = 0; j < n; j++) {
			z[i+j] += x[i]*y[j];
		}
	}
	
	reduce_to_digits(z, max);
	
	int ret = largeint_length(z,max);
	
	return ret; /*returns length of output */
}




