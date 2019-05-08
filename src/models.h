#include<stdio.h>
#include"dataset.h"
#include <stdlib.h>
#include<string.h>

#ifndef MODELS
#define MODELS


// Gerekli veri tanimlarini burada yapabilirsiniz


int model_by_similarity(House* houses,House new_house);
void create_data_matrices(House* houses,int** X,int* y);
int** get_transpose(int** A);
int** get_inverse(int** A);
int** get_multiplication(int** A, int** B);
int** calculate_parameter(House* houses);

#endif
