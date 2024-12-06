/*
 *NAME
 * matrix-multiplier : multiples two matrices and prints out the product
 *
 * Author: Timur Boskailo
 * Date: 12-6-24
 *
 *SYNOPSIS
 * matrix-multiplier <# rows left matrix> <# columns left matrix>
 *                   <# rows right matrix> <# columns right matrix>
 *
 *DESCRIPTION
 * The user specifies the dimensions of the two matrices through the command
 * line and their entries are randomly generated. The product of the matricies
 * is computed and the result is printed to stdout.
 *
 *FUNCTIONS
 * matrix_init()        initializes matrix structure
 * free_matrix()        frees allocated memory associated with matrix structure
 * mulitply_row()       computes one row of matrix product
 * fill_with_random()   populates matrix with random decimal values 0<= x < 1
 * print_matrix()       prints out matrix
 *
 *
 *
 */
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
 * Return: Void
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
 *  Return: Void
 *  Algorithm:
 *  1.For each column in B, calculate the dot product of row 'row'
 *      of A with the column in B
 *  2. Store result in C
 */
void multiply_row(Matrix *A, Matrix *B, Matrix *C, int row)
{
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

/*
 * Function: fill_with_random()
 * Purpose: populate a matrix with random decimal values between 0 and 1
 * Argument: pointer to Matrix structure
 * Return: void
 */
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

/*
 * Function: print_matrix()
 * Purpose: prints a formatted matrix
 * Argument: pointer to Matrix structure
 * Return: void
 */
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

/*
 * Main function checks for command line usage errors. If no errors, saves
 * argv entries as integers and uses them to create two matrices A and B
 * with dimensions A (argv[1], argv[2]) and B (argv[3], argv[4]).
 * Then it computes the product of A and B, stores the result in matrix C,
 * and prints out the result.
 */
int main(int argc, char **argv)
{

    if (argc != 5)
    {
        fprintf(stderr, "Error: Incorrect number of arguments.\n"
                        "Usage: matrix-mult <# rows left matrix> "
                        "<# columns left matrix> <# rows right matrix> "
                        "<# columns right matrix>\n");
        return EXIT_FAILURE;
    }

    int rows_A = atoi(argv[1]);
    int columns_A = atoi(argv[2]);
    int rows_B = atoi(argv[3]);
    int columns_B = atoi(argv[4]);

    if (rows_A <= 0 || columns_A <= 0 || rows_B <= 0 || columns_B <= 0)
    {
        fprintf(stderr, "Error: Matrix dimension values must be "
                        "positive integers.\n");
        return EXIT_FAILURE;
    }

    if (columns_A != rows_B)
    {
        fprintf(stderr, "Error: Matrix dimensions:\n # of columns in "
                        "left matrix must equal # of rows in right matrix\n.");
        return EXIT_FAILURE;
    }

    // seed number generator to change randomization each time program run
    srand(time(NULL));

    Matrix *matrix_A = matrix_init(rows_A, columns_A);
    Matrix *matrix_B = matrix_init(rows_B, columns_B);
    Matrix *matrix_C = matrix_init(rows_A, columns_B);

    fill_with_random(matrix_A);
    fill_with_random(matrix_B);

    for (int i = 0; i < rows_A; i++)
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
