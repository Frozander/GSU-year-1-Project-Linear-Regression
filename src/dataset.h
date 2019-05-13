#include<stdio.h>
#include <stdlib.h>
#include<string.h>

#ifndef DATASET
#define DATASET

//hash table oluşturuken (place_house fonksyonu) fonksyon girdisini kolaylaştırmak için hard coded veriler
#define HASH_TYPE_ID 1
#define HASH_TYPE_NEIGHBORHOODS 2

//print house fonksyonu için hardcode seçenekler
#define SINGLE_WITH_TOP 1
#define SINGLE_WITHOUT_TOP 0
#define MULTI 2
#define JUST_TOP 3
#define LIMITLESS 0

//Daha kolay sorting için hardcoded int değerler
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
  struct house * nextHouseById;
  struct house * nextHouseByNeighbor;
} House;

House * linearise_hash_table (House * ht[], int hash_type);
void read_house_data(char* filename, House * hById[], House * hByN[]);
void create_hash_table(House * house_list_head, House * houses[], int hash_type);
House* get_neighborhoods(House * house, House * houses[]);
void print_house(House * house, int style, int limit);
House* get_house_byid(int id, House * houses[]);
void mean_sale_prices(House* houses_head,int criter_name);
void sort_houses(House** houses, int criter_name);
void create_hash_table_tree(House * houses[], int hash_type);
int convert_kitchenqual (char * c);
char * convert_kitchenqual_back (int value);



#endif
