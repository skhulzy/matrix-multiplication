#include <stdio.h>
#include <stdlib.h>
// provides system time, used with rand()
#include <time.h>

/*
 * matrix structure
 * - stores a matrix
 * NOTE: dynamically allocate matrix structures with matrix_init()
 */

typedef struct
{
    int num_rows;
    int num_columns;
    double **data;
} Matrix;

/*
 * Function: matrix_init()
 * Purpose: dynamically allocate a matrix structure
 * Arguments: number of rows, number of columns
 * Return: a pointer to the matrix
 */
Matrix *matrix_init(int rows, int columns)
{
    Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
    if (matrix == NULL)
    {
        perror("Memory allocation of matrix structure failed\n");
        exit(EXIT_FAILURE);
    } // memory allocation failed

    matrix->num_rows = rows;
    matrix->num_columns = columns;

    // allocate row pointers
    matrix->data = (double **)malloc(rows * sizeof(double *));
    if (matrix->data == NULL)
    {
        perror("memory allocation of row pointers failed\n");
        free(matrix);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++)
    {
        matrix->data[i] = (double *)malloc(columns * sizeof(double));
        if (matrix->data[i] == NULL)
        {
            perror("memory allocation of row failed\n");
            // free previously allocated pointers
            for (int j = 0; j < i; j++)
            {
                free(matrix->data[j]);
            }
            free(matrix->data);
            free(matrix);
            exit(EXIT_FAILURE);
        }
    }

    return matrix;
}

/*
 * Function: free_matrix
 * Purpose: free all allocated memory associated with a matrix structure
 * Argument: matrix structure
 * Return: VOID
 */
void free_matrix(Matrix *matrix)
{
    for (int i = 0; i < matrix->num_rows; i++)
    {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

/*
 * Function: multiply_row
 * Purpose: Partially multiply matricies A and B to compute
 *  firt row of product matrix C
 * Arguments: Matrix *A - pointer to left factor
 *            Matrix *B - pointer to right factor
 *            Matrix *C - pointer to product
 *            int row - row in matrix A that will be mulitplied
 *                      and stored in same row of C
 *  Return: NULL
 *  Algorithm:
 *  1.For each column in B, calculate the dot product of row 'row'
 *      of A with the column in B
 *  2. Store result in C
 */
void multiply_row(Matrix *A, Matrix *B, Matrix *C, int row)
{
    // take out later once command line code added,
    // and move size check to main()
    if (A->num_columns != B->num_rows)
    {
        perror("Matrix multiplication failure: # of columns in A != # of"
               "rows in B\n");
        free_matrix(A);
        free_matrix(B);
        free_matrix(C);
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < B->num_columns; j++)
    {
        // initialize every element in C's current row to 0 to avoid errors
        // there could be garbage values in the memory location
        C->data[row][j] = 0;

        for (int i = 0; i < A->num_columns; i++)
        {
            C->data[row][j] += A->data[row][i] * B->data[i][j];
        }
    }
}

void fill_with_random(Matrix *matrix)
{
    for (int i = 0; i < matrix->num_rows; i++)
    {
        for (int j = 0; j < matrix->num_columns; j++)
        {
            matrix->data[i][j] = (double)rand() / (double)RAND_MAX;
        }
    }
}

void print_matrix(Matrix *matrix)
{
    for (int i = 0; i < matrix->num_rows; i++)
    {
        for (int j = 0; j < matrix->num_columns; j++)
        {
            printf("%.2f ", matrix->data[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    // seed number generator to change randomization each time program run
    srand(time(NULL));

    Matrix *matrix_A = matrix_init(5, 5);
    Matrix *matrix_B = matrix_init(5, 5);
    Matrix *matrix_C = matrix_init(5, 5);

    fill_with_random(matrix_A);
    fill_with_random(matrix_B);

    for (int i = 0; i < 5; i++)
    {
        multiply_row(matrix_A, matrix_B, matrix_C, i);
    }

    printf("Left Matrix\n");
    print_matrix(matrix_A);

    printf("Right Matrix\n");
    print_matrix(matrix_B);

    printf("Product Matrix\n");
    print_matrix(matrix_C);

    free_matrix(matrix_A);
    free_matrix(matrix_B);
    free_matrix(matrix_C);

    return 0;
}
