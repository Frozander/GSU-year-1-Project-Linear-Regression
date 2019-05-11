#include<stdio.h>
#include <stdlib.h>
#include<string.h>

#ifndef DATASET
#define DATASET

#define ID 0
#define LOTAREA 1
#define STREET 2
#define SALEPRICE 3
#define NEIGHBORHOOD 4
#define YEARBUILT 5
#define OVERALLQUAL 6
#define OVERALLCOND 7
#define KITCHENQUAL 8

/*
 * Ornek ev veri yapisi
 * Bu veri yapisini kullanabilir
 * ya da kendiniz yeni bir yapi 
 * kodlayabilirsiniz.  
 */

typedef struct house{
  int id;
  int lotarea;
  char street[25];
  int saleprice;
  char neighborhood[25];
  int yearbuilt;
  int overallqual;
  int overallcond;
  int kitchenqual;
  struct house * nextHouse;
} House;


void read_house_data(char* filename, House* houses[]);
void print_house(House house);
House get_house_byid(int id, House * houses[]);
House* get_neighborhoods(House house);
float* mean_sale_prices(House* houses,char* criter_name);
void sort_houses(House* houses,char* criter_name);
void create_hash_table_tree(House * houses[] );
int convert_kitchenqual (char * c);
char * convert_kitchenqual_back (int value);
House* merge_sort(House ** list, int style);
void split_list(House* input, House** first_half, House** second_half);
House* merge(House* in1, House* in2, int style);



#endif
