#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/*
	Compile this program using:	
	cc -o 2039276_Task2_B 2039276_Task2_B.c -lpthread

	Run this program using:
	./2039276_Task2_B
	
	 Sonam Wangdi Sherpa, UID: 2039276

*/
#define MAT_SIZE 1024

int i,j,k;           //Parameters For Rows And Columns
int matrix1[MAT_SIZE][MAT_SIZE]; //First Matrix
int matrix2[MAT_SIZE][MAT_SIZE]; //Second Matrix
int result [MAT_SIZE][MAT_SIZE]; //Multiplied Matrix

//Type Defining For Passing Function Argumnents
typedef struct parameters {
    int x,y;
}args;

//Function For Calculate Each Element in Result Matrix Used By Threads - - -//
void* thread_Multiply_Matrix(void* arg){
    
    args* p = arg;
    
    //Calculating Each Element in Result Matrix Using Passed Arguments
    for(int a=0;a<j;a++){
        result[p->x][p->y] += matrix1[p->x][a]*matrix2[a][p->y];
    }
    sleep(3);
    
    //End Of Thread
    pthread_exit(0);
}


int main(){
    int x,y;        
    int MAX_THREADS;
    struct timespec start, finish;   
    long long int time_elapsed;

	printf("\nEnter size of MAX_THREADS\n");
	scanf("%d", &MAX_THREADS);
    
    //Getting Matrix1 And Matrix2 Info from User - - - - - - - - - - - - - - -//
    
    printf(" --- Defining Matrix 1 ---\n\n");
    
    // Getting Row And Column(Same As Row In Matrix2) Number For Matrix1
    printf("Enter number of rows for matrix 1: ");
    scanf("%d",&i);
    printf("Enter number of columns for matrix 1: ");
    scanf("%d",&j);
    
    printf("\n --- Initializing Matrix 1 ---\n\n");
    for(int x=0;x<i;x++){
        for(int y=0;y<j;y++){
            matrix1[x][y] =rand()%2;
        }
    }
    
    printf("\n --- Defining Matrix 2 ---\n\n");

    // Getting Column Number For Matrix2
    printf("Number of rows for matrix 2 : %d\n",j);
    printf("Enter number of columns for matrix 2: ");
    scanf("%d",&k);
    
    printf("\n --- Initializing Matrix 2 ---\n\n");
    for(int x=0;x<j;x++){
        for(int y=0;y<k;y++){
            matrix2[x][y]=rand()%2;
        }
    }
    
    
    //Printing Matrices - - - - - - - - - - - - - - - - - - - - - - - - - - -//
    
    // printf("\n --- Matrix 1 ---\n\n");
    // for(int x=0;x<i;x++){
    //     for(int y=0;y<j;y++){
    //         printf("%5d",matrix1[x][y]);
    //     }
    //     printf("\n\n");
    // }
    
    // printf(" --- Matrix 2 ---\n\n");
    // for(int x=0;x<j;x++){
    //     for(int y=0;y<k;y++){
    //         printf("%5d",matrix2[x][y]);
    //     }
    //     printf("\n\n");
    // }
    
    clock_gettime(CLOCK_MONOTONIC, &start);

    //Multiply Matrices Using Threads - - - - - - - - - - - - - - - - - - - -//
    
    //Defining Threads
    pthread_t thread[MAX_THREADS];
    
    //Counter For Thread Index
    int thread_number = 0;
    
    //Defining p For Passing Parameters To Function As Struct
    args p[i*k];
    
    for(int x=0;x<i;x++){
        for(int y=0;y<k;y++){
            
            //Initializing Parameters For Passing To Function
            p[thread_number].x=x;
            p[thread_number].y=y;
            
            //Status For Checking Errors
            int status;
            
            //Create Specific Thread For Each Element In Result Matrix
            status = pthread_create(&thread[thread_number], NULL, thread_Multiply_Matrix, (void *) &p[thread_number]);
            
            //Check For Error
            if(status!=0){
                printf("Error In Threads");
                exit(0);
            }
            
            thread_number++;
        }
    }
    
    
    //Wait For All Threads Done - - - - - - - - - - - - - - - - - - - - - - //
    
    for(int z=0;z<(i*k);z++)
        pthread_join(thread[z],NULL );
    
    
    //Print Multiplied Matrix (Result) - - - - - - - - - - - - - - - - - - -//
    
    printf(" --- Multiplied Matrix ---\n\n");
    for(int x=0;x<i;x++){
        for(int y=0;y<k;y++){
            printf("%5d",result[x][y]);
        }
        printf("\n\n");
    }
    
    printf(" ---> Used Threads : %d \n\n",thread_number);
    
    
    clock_gettime(CLOCK_MONOTONIC, &finish);
    time_difference(&start, &finish, &time_elapsed);
    printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
                                         (time_elapsed/1.0e9)); 
    
    return 0;
}

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
