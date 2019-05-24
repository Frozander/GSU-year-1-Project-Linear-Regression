#include<stdio.h>
#include <stdlib.h>
#include<string.h>

#ifndef DATASET
#define DATASET

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
#define NEXT_HOUSE 9
#define NEXT_HOUSE_BY_ID 10
#define NEXT_HOUSE_BY_NEIGHBORHOOD 11

//Artan veya Azalan olarak sıralamak için hardcoded değerler
#define ASC 0
#define DESC 1

//limit_houses için hardcode değerler
#define NON -1

//file types
#define TEST 1
#define TRAIN 0

//operators


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
void create_hash_table(House * house_list_head, House * houses[], int hash_type);
House* get_neighborhoods(House * house, House * houses[]);
void print_house(House * house, int style, int limit);
House* get_house_byid(int id, House * houses[]);
void mean_sale_prices(House* houses_head, int criter_name, int criter_data);
void sort_houses(House** houses, int criter_name, int order);
void create_hash_table_tree(House * houses[], int hash_type);
int convert_kitchenqual (char * c);
char * convert_kitchenqual_back (int value);
void merge_sort(House ** list, int criter_name, int order);
void split_list(House* input, House** first_half, House** second_half);
House* merge(House* in1, House* in2, int criter_name, int order);
House** pull_from_table_by_id_as_headref(int id, House* house_list[]);
void print_list(House* list_input);
char * ghc_s (House * house, int criter_name);
int ghc_i (House * house, int criter_name);
House * ghc_p (House * house, int criter_name);
House* limit_list(House* houses_head, int criter_name, int min, int max);
void read_house_data(char* filename, House * hById[], House * hByN[], int file_type);
void limit_houses(House** houses_head, int criter_name, int min, int max);
int get_criter_avg(House* head, int criter);
int get_list_lenght (House * head);
#endif
