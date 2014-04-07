/* Example code for Software Systems at Olin College.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    double **data;
    int rows, cols;
} Matrix;

void null_pointer_catch(void *p, char *msg) {
    if (p == NULL) {
        perror(msg);
        exit(-1);
    }
}

// Makes a new matrix and sets all elements to zero.
Matrix *make_matrix(int rows, int cols) {
    // TODO: Fill this in
    Matrix *M = (Matrix *)malloc(sizeof(Matrix));
    null_pointer_catch(M,"Malloc failed");
    M->rows = rows;
    M->cols = cols;
    M->data = (double **)malloc(sizeof(double *)*M->rows);
    null_pointer_catch(M->data,"Malloc failed");
    int i,j;
    for (i=0; i<M->rows; i++) {
        M->data[i] = (double *)malloc(sizeof(double)*M->cols);
        null_pointer_catch(M->data[i],"Malloc failed");
        for (j = 0; j<M->cols; j++) {
            M->data[i][j] = 0;
        }
    }
    return M;
}

// Prints the elements of a matrix.
void print_matrix(Matrix *matrix) {
    int i, j;

    for (i=0; i<matrix->rows; i++) {
	for (j=0; j<matrix->cols; j++) {
	    printf("%lf ", matrix->data[i][j]);
	}
	printf("\n");
    }
}

// Adds a scalar to all elements of a matrix.
void increment_matrix(Matrix *matrix, int incr) {
    int i, j;

    for (i=0; i<matrix->rows; i++) {
	for (j=0; j<matrix->cols; j++) {
	    matrix->data[i][j] += incr;
	}
    }
}

// Sets the elements of a matrix to consecutive numbers.
void consecutive_matrix(Matrix *matrix) {
    int i, j;

    for (i=0; i<matrix->rows; i++) {
	for (j=0; j<matrix->cols; j++) {
	    matrix->data[i][j] = i * matrix->cols + j;
	}
    }
}

// Adds two matrices elementwise and stores the result in the given
// destination matrix (C).
void add_matrix(Matrix *A, Matrix *B, Matrix *C) {
    int i, j;

    assert(A->rows == B->rows && B->rows == C->rows);
    assert(A->cols == B->cols && B->cols == C->cols);

    for (i=0; i<A->rows; i++) {
	for (j=0; j<A->cols; j++) {
	    C->data[i][j] =  A->data[i][j] + B->data[i][j];
	}
    }
}

// Adds two matrices elementwise and returns a new matrix.
Matrix *add_matrix_func(Matrix *A, Matrix *B) {
    Matrix *C = make_matrix(A->rows, A->cols);
    add_matrix(A, B, C);
    return C;
}

// Performs matrix multiplication and stores the result in the given
// destination matrix (C).
void mult_matrix(Matrix *A, Matrix *B, Matrix *C) {
    // TODO: Fill this in
    // Note that it is asking for matrix multiplication, not
    // elementwise multiplication
    // C[i][j] = A row i * B col j
    int i, j, k;
    double cell;
    assert(A->rows == B->cols && A->rows == C->rows);
    assert(A->cols == B->rows && B->cols == C->cols);
    
    for (i = 0; i < A->rows; i++){
        for (j = 0; j < B->cols; j++) {
            cell = 0;
            for (k = 0; k < A->cols; k++) {
                // dot products!
                cell += (A->data[i][k])*(B->data[k][j]);
            }
            C->data[i][j] = cell;
        }
    }
}

// Performs matrix multiplication and returns a new matrix.
Matrix *mult_matrix_func(Matrix *A, Matrix *B) {
    Matrix *C = make_matrix(A->rows, B->cols);
    mult_matrix(A, B, C);
    return C;
}

int main() {
    Matrix *A = make_matrix(3, 4);
    null_pointer_catch(A,"make_matrix failed");
    consecutive_matrix(A);
    printf("A\n");
    print_matrix(A);
    
    Matrix *C = add_matrix_func(A, A);
    printf("\nC = A + A\n");
    print_matrix(C);
    
    Matrix *B = make_matrix(4, 3);
    increment_matrix(B, 1);
    printf("\nB\n");
    print_matrix(B);

    Matrix *D = mult_matrix_func(A, B);
    printf("\nD = A*B\n");
    print_matrix(D);
    
    return(0);
}
