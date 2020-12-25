#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>
/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o 2039276_Task2_C_5 2039276_Task2_C_5.c -lcrypt -pthread

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./thread_rukesh1e > threadrukesh_results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/


/**
 Required by lack of standard function in C.   
*/
int count=0;
pthread_t t1,t2;

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void thread_possix(char *passs)
{
	void *kernel_function_1();
	void *kernel_function_2();
	
	
		pthread_create(&t1, NULL, kernel_function_1, passs);
		pthread_join(t1, NULL);
		pthread_create(&t2, NULL, kernel_function_2, passs);
		pthread_join(t2, NULL);


		
}



void *kernel_function_1(char *salt_and_encrypted){
  int r, u, k;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password  // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(r='A'; r<='M'; r++){
    for(u='A'; u<='Z'; u++){
      for(k=0; k<=99; k++){
        sprintf(plain, "%c%c%02d", r, u, k); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
          
        } 
        
      }
    }
  }
}


void *kernel_function_2(char *salt_and_encrypted){
  int r, u, k;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password  // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(r='N'; r<='Z'; r++){
    for(u='A'; u<='Z'; u++){
      for(k=0; k<=99; k++){
        sprintf(plain, "%c%c%02d", r, u, k); 
        enc = (char *) crypt(plain, salt);

        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
          
        } 
      }
    }
  }
}
int time_difference(struct timespec *start, struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}


int main(int argc, char *argv[]) {

  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start); 
  thread_possix("$6$AS$hlfnAQW5GoL87atDkXIGdn0cKsJdL2u9ZJIMzieZYHxZ8hU2Uw1ugXU2XI6Oy4pVymkvcbgnjYJhmmgCFeXm9.");
  printf("%d solutions explored\n", count);
  clock_gettime(CLOCK_MONOTONIC, &finish); 
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 

  return 0;
}

