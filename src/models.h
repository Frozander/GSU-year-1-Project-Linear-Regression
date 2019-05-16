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
    int collmuns;
    // Two dimensional array for values
    double** values;
} Matrix;

int model_by_similarity(House* houses,House new_house);
void create_data_matrices(House* houses,Matrix* X,Matrix* Y);
Matrix* get_transpose(Matrix* A);
Matrix* get_inverse(Matrix* A);
Matrix* get_multiplication(Matrix* A, Matrix* B);
Matrix* calculate_parameter(House* houses);

Matrix* create_matrix(int line, int collumn);

#endif
