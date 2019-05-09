#include<stdio.h>
#include <stdlib.h>
#include<string.h>

#ifndef DATASET
#define DATASET


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



#endif
