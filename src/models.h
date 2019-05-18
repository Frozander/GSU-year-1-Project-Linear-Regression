#include<stdio.h>
#include"dataset.h"
#include <stdlib.h>
#include<string.h>

#ifndef MODELS
#define MODELS


// Gerekli veri tanimlarini burada yapabilirsiniz
typedef struct matrix
{
    // Lines and collumns for the matrix
    int lines;
    int columns;
    // Two dimensional array for values
    double** values;
} Matrix;

int model_by_similarity(House * houses [], House  * house);
void create_data_matrices(House** houses,Matrix** X,Matrix** Y);
Matrix* get_transpose(Matrix* A);
Matrix* get_inverse(Matrix* A);
Matrix* get_multiplication(Matrix* A, Matrix* B);
Matrix* calculate_parameter(House* houses);
Matrix* make_prediction(House** house_in,Matrix* W);
Matrix* create_matrix(int line, int collumn);
void print_matrix(Matrix* matrix_in);
void free_matrix(Matrix* matrix_in);
void file_write_house(House** house_in, char* filename);
void matrix_to_house_list(Matrix* matrix_in, House** house_out);

#endif
