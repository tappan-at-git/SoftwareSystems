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


// Makes a new matrix and sets all elements to zero.
Matrix *make_matrix(int rows, int cols) {
    int i;
    Matrix *matrix = malloc(sizeof(Matrix));

    matrix->data = malloc(rows * sizeof(double *));
    matrix->rows = rows;
    matrix->cols = cols;

    for (i=0; i<rows; i++) {
	// use calloc to initialize to 0
	matrix->data[i] = calloc(cols, sizeof(double));
    }
    return matrix;
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
    int i, j, k;

    assert(A->rows == B->cols);
    assert(A->rows == C->rows);
    assert(B->cols == C->cols);

    for (i=0; i<C->rows; i++) {
	for (j=0; j<C->cols; j++) {
	    for (k=0; k<A->cols; k++) {
		C->data[i][j] += A->data[i][k] * B->data[k][j];
	    }
	}
    }
}

// Performs matrix multiplication and returns a new matrix.
Matrix *mult_matrix_func(Matrix *A, Matrix *B) {
    Matrix *C = make_matrix(A->rows, B->cols);
    mult_matrix(A, B, C);
    return C;
}


/* In matrix2.c there are two versions of matrix_sum: one traverses the rows
 * and one traverses the columns. Assuming that the matrix is bigger than the 
 * memory cache and that a memory cache line is big enough to hold 8 matrix
 * elements, what cache hit rate would you expect for each version of
 * matrix_sum? Explain your reasoning.
 */
/* A = array of doubles
 * cache line = 8 matrix elements
 * 1 miss at each new row, 7 hits, 1 miss...
 * sum1 is the "Best" case, with 1 miss/7 hits in cache line + a good chance of
 * pre-emptively loading spatially localized data into the cache.
 * sum2 is the worst case, with a lot of misses.
 * If the cache is small enough we may need to cache new data every fetch.
 * Otherwise, we'll have some data in the cache from the last time we were in the
 * row...still a miss, but at least not as big a miss.
 */
double matrix_sum1(Matrix *A) {
    double total = 0.0;
    int i, j;

    for (i=0; i<A->rows; i++) {
	for (j=0; j<A->cols; j++) {
	    total += A->data[i][j];
	}
    }
    return total;
}
    
double matrix_sum2(Matrix *A) {
    double total = 0.0;
    int i, j;

    for (j=0; j<A->cols; j++) {
	for (i=0; i<A->rows; i++) {
	    total += A->data[i][j];
	}
    }
    return total;
}
    

// Adds up the rows of A and returns a heap-allocated array of doubles.
double *row_sum(Matrix *A) {
    double total;
    int i, j;

    double *res = malloc(A->rows * sizeof(double));

    for (i=0; i<A->rows; i++) {
	total = 0.0;
	for (j=0; j<A->cols; j++) {
	    total += A->data[i][j];
	}
	res[i] = total;
    }
    return res;
}

// Adds up the columns of A and returns a heap-allocated array of doubles.
double *col_sum(Matrix *A) {
    double total;
    int i, j;
    
    double *res = malloc(A->rows * sizeof(double));
    
    for (i=0; i<A->cols; i++) {
        total = 0.0;
        for (j=0; j<A->rows; j++) {
            total += A->data[j][i];
        }
        res[i] = total;
    }
    return res;
}

// Adds up the diagonals of A and returns a heap-allocated array of doubles.
double *diag_sum(Matrix *A) {
    int i;
    
    // Unlike columns and rows, A will only have 2 (forward and backward)
    // diagonals.
    double *res = malloc(2 * sizeof(double));
    res[0] = 0.0;
    res[1] = 0.0;
    
    // A is not *necessarily* a square matrix
    for (i=0; (i < A->rows) && (i < A->cols); i++) {
        res[0] += A->data[i][i];
        res[1] += A->data[A->rows-1 - i][A->cols-1 - i];
    }
    return res;
}


int is_uniform_vector(double *V, int len, double base) {
    int i;
    for (i = 1; i < len; i++) {
        if (V[i] != base) {
            return 0;
        }
    }
    return 1;
}

/* 
   http://en.wikipedia.org/wiki/Magic_square

   A magic square is an arrangement of numbers (usually integers) in a
   square grid, where the numbers in each row, and in each column, and
   the numbers in the forward and backward main diagonals, all add up
   to the same number. 

   Write a function called is_magic_square() that takes a matrix and 
   returns an int, 1 if the matrix is a magic square, and 0 otherwise.

   Feel free to use row_sum().
*/
int is_magic_square(Matrix *M) {
    // adding rows should be fastest due to caching, check that first
    double *row_sums = row_sum(M);
    double magnum = row_sums[0];
    if (!is_uniform_vector(row_sums,M->rows,magnum)) {
        return 0;
    }
    free(row_sums); // tidy tidy
    
    // adding columns is slower, since it's more likely for the data to be
    // spatially distant
    double *col_sums = col_sum(M);
    if (!is_uniform_vector(col_sums,M->cols,magnum)) {
        return 0;
    }
    free(col_sums);
    
    double *diag_sums = diag_sum(M);
    return (diag_sums[0] == magnum) && (diag_sums[1] == magnum);
}


int main() {
    int i;

    Matrix *A = make_matrix(3, 4);
    consecutive_matrix(A);
    printf("A\n");
    print_matrix(A);

    Matrix *C = add_matrix_func(A, A);
    printf("A + A\n");
    print_matrix(C);

    Matrix *B = make_matrix(4, 3);
    increment_matrix(B, 1);
    printf("B\n");
    print_matrix(B);

    Matrix *D = mult_matrix_func(A, B);
    printf("D\n");
    print_matrix(D);
    

    double sum = matrix_sum1(A);
    printf("sum(A) = %lf\n", sum);

    sum = matrix_sum2(A);
    printf("sum2(A) = %lf\n", sum);

    double *sums = row_sum(A);
    for (i=0; i<A->rows; i++) {
	printf("row %d\t%lf\n", i, sums[i]);
    }
    // should print 6, 22, 38

    
    Matrix *E = make_matrix(3,3);
    increment_matrix(E, 1);
    printf("\nE ");
    if (is_magic_square(E)) {
        printf("is magic! Whizzam!\n");
    } else {
        printf("isn't magic...\n");
    }
    print_matrix(E);
    return 0;
}
