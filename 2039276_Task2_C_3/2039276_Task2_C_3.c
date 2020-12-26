#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>
#include <time.h>

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 3 uppercase
  letters and a 2 digit integer.

  Compile with:
    cc -o 2039276_Task2_C_3 2039276_Task2_C_3.c -lcrypt

  If you want to analyse the output then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:
    ./2039276_Task2_C_3 > output.txt

  Sonam Wangdi Sherpa, UID:2039376
******************************************************************************/

int count=0;     // A counter used to track the number of combinations explored so far


void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void crack(char *salt_and_encrypted){
  int a,b,c,d;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
  char plain[7];   // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
  char *enc;       // Pointer to the encrypted password

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='A'; a<='Z'; a++){
    for(b='A'; b<='Z'; b++){
      for(c='A'; c<='Z'; c++){
        for(d=0; d<=99; d++){
          sprintf(plain, "%c%c%c%02d", a, b, c, d); 
          enc = (char *) crypt(plain, salt);
          count++;
          if(strcmp(salt_and_encrypted, enc) == 0){
	    printf("#%-8d%s %s\n", count, plain, enc);
          } 
        } 
      }
    }
  }
}

// Calculate the difference between two times. Returns zero on
// success and the time difference through an argument. It will 
// be unsuccessful if the start time is after the end time.

int time_difference(struct timespec *start, 
                    struct timespec *finish, 
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

int main(int argc, char *argv[]){
  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
 
  crack("$6$AS$kxt4Rj8.ceicJCG.Ug3KHfo8QOb6MWfTFzGxW.U8iGYAAWfJxyUqS2Q3weNH2fWRnTc8tPVvlfJUtB9M9BQXf.");
  printf("%d solutions explored\n", count);

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
                                         (time_elapsed/1.0e9)); 
  

  return 0;
}

