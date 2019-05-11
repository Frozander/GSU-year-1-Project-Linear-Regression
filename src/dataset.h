#include<stdio.h>
#include <stdlib.h>
#include<string.h>

#ifndef DATASET
#define DATASET

//hash table oluşturuken (place_house fonksyonu) fonksyon girdisini kolaylaştırmak için hard coded veriler
#define HASH_TYPE_ID 1
#define HASH_TYPE_NEIGHBORHOODS 2


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
  struct house * nextHouseById;
  struct house * nextHouseByNeighbor;
} House;


void read_house_data(char* filename, House * hById[], House * hByN[]);
void create_hash_table(House * house_list_head, House * houses[], int hash_type);
House* get_neighborhoods(House * house, House * houses[]);
void print_house(House house);
House get_house_byid(int id, House * houses[]);
void mean_sale_prices(House* houses_head,int criter_name);
void sort_houses(House* houses, int criter_name);
void create_hash_table_tree(House * houses[], int hash_type);
int convert_kitchenqual (char * c);
char * convert_kitchenqual_back (int value);



#endif
