#include <stdio.h>
#include <stdlib.h>

/*
 * matrix structure
 * - stores a matrix
 * - dynmamically allocate matrix structures with matrix_init()
 */

typedef struct
{
    int num_rows;
    int num_columns;
    double **data;
} Matrix;

/*
 * function: matrix_init()
 * Purpose: dynamically allocate a matrix structure
 * arguments: number of rows, number of columns
 * return: a pointer to the matrix
 *
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

void free_matrix(Matrix *matrix)
{
    for (int i = 0; i < matrix->num_rows; i++)
    {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

int main(int argc, char **argv)
{

    return 0;
}
