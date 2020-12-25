#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <time.h>

/*
to compile :
gcc -o 2039276_Task2_A 2039276_Task2_A.c 

to run:
./2039276_Task2_A

*/


int **matrix_allocate(int row, int column) {

    int **matrix = malloc(row * sizeof(*matrix));

    for (int i = 0; i < row; i++) {

        matrix[i] = calloc(column, sizeof(*matrix[i]));

    }

    return matrix;

}



void matrix_free(int **matrix, int row) {

    for (int i = 0; i < row; i++) {

        free(matrix[i]);

    }

    free(matrix);

}



void matrix_print(const char *str, int **a, int row) {

    int min, max, w = 0, n1, n2, nw;

    min = max = a[0][0];

    for (int i = 0; i < row; i++) {

        for (int j = 0; j < row; j++) {

            if (min > a[i][j])

                min = a[i][j];

            if (max < a[i][j])

                max = a[i][j];

        }

    }

    n1 = snprintf(NULL, 0, "%d", min);

    n2 = snprintf(NULL, 0, "%d", max);

    nw = n1 > n2 ? n1 : n2;



    for (int i = 0; i < row; i++) {

        if (i == 0)

            w = printf("%s = ", str);

        else

            printf("%*s", w, "");



        for (int j = 0; j < row; j++) {

            printf(" %*d", nw, a[i][j]);

        }

        printf("\n");

    }

    fflush(stdout);

}



int **matrix_add(int **a, int **b, int row, int deallocate) {

    int **c = matrix_allocate(row, row);

    for (int i = 0; i < row; i++) {

        for (int j = 0; j < row; j++) {

            c[i][j] = a[i][j] + b[i][j];

        }

    }

    if (deallocate & 1) matrix_free(a, row);

    if (deallocate & 2) matrix_free(b, row);



    return c;

}



int **matrix_multiply(int **A, int **B, int row, int deallocate) {

    int **C = matrix_allocate(row, row);

    if (row == 1) {

        C[0][0] = A[0][0] * B[0][0];

    } else {

        int row2 = row / 2;

        int **a11 = matrix_allocate(row2, row2);

        int **a12 = matrix_allocate(row2, row2);

        int **a21 = matrix_allocate(row2, row2);

        int **a22 = matrix_allocate(row2, row2);

        int **b11 = matrix_allocate(row2, row2);

        int **b12 = matrix_allocate(row2, row2);

        int **b21 = matrix_allocate(row2, row2);

        int **b22 = matrix_allocate(row2, row2);



        for (int i = 0; i < row2; i++) {

            for (int j = 0; j < row2; j++) {

                a11[i][j] = A[i][j];

                a12[i][j] = A[i][j + row2];

                a21[i][j] = A[i + row2][j];

                a22[i][j] = A[i + row2][j + row2];

                b11[i][j] = B[i][j];

                b12[i][j] = B[i][j + row2];

                b21[i][j] = B[i + row2][j];

                b22[i][j] = B[i + row2][j + row2];

            }

        }



        int **c11 = matrix_add(matrix_multiply(a11, b11, row2, 0),

                               matrix_multiply(a12, b21, row2, 0), row2, 1+2);

        int **c12 = matrix_add(matrix_multiply(a11, b12, row2, 1),

                               matrix_multiply(a12, b22, row2, 1), row2, 1+2);

        int **c21 = matrix_add(matrix_multiply(a21, b11, row2, 2),

                               matrix_multiply(a22, b21, row2, 2), row2, 1+2);

        int **c22 = matrix_add(matrix_multiply(a21, b12, row2, 1+2),

                               matrix_multiply(a22, b22, row2, 1+2), row2, 1+2);



        for (int i = 0; i < row2; i++) {

            for (int j = 0; j < row2; j++) {

                C[i][j] = c11[i][j];

                C[i][j + row2] = c12[i][j];

                C[i + row2][j] = c21[i][j];

                C[i + row2][j + row2] = c22[i][j];

            }

        }

        matrix_free(c11, row2);

        matrix_free(c12, row2);

        matrix_free(c21, row2);

        matrix_free(c22, row2);

    }

    if (deallocate & 1) matrix_free(A, row);

    if (deallocate & 2) matrix_free(B, row);



    return C;

}



int **matrix_multiply_direct(int **A, int **B, int row, int deallocate) {

    int **C = matrix_allocate(row, row);

    for (int i = 0; i < row; i++) {

        for (int j = 0; j < row; j++) {

            int x = 0;

            for (int k = 0; k < row; k++) {

                x += A[i][k] * B[k][j];

            }

            C[i][j] = x;

        }

    }

    if (deallocate & 1) matrix_free(A, row);

    if (deallocate & 2) matrix_free(B, row);



    return C;

}



int main(int argc, char **argv) {

    int n = argc < 2 ? 8 : atoi(argv[1]);

    int **A = matrix_allocate(n, n);

    int **B = matrix_allocate(n, n);



    for (int i = 0; i < n; i++) {

        for (int j = 0; j < n; j++) {

            A[i][j] = i + j;

            B[i][j] = i + j;

        }

    }



    matrix_print("A", A, n);

    matrix_print("B", B, n);



    if ((n & (n - 1)) == 0) {

        /* recursive method can be applied only to powers of 2 */

        clock_t ticks = -clock();

        int **C = matrix_multiply(A, B, n, 0);

        ticks += clock();

        matrix_print("C = A * B", C, n);

        printf("%ld ticks\n", ticks);

        matrix_free(C, n);

    }



    clock_t ticks = -clock();

    int **D = matrix_multiply_direct(A, B, n, 1+2);

    ticks += clock();



    matrix_print("D = A * B", D, n);

    printf("%ld ticks\n", ticks);

    matrix_free(D, n);



    return 0;
}
