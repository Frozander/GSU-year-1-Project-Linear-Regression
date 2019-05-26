#include<stdio.h>
#include <stdlib.h>
#include<string.h>

#ifndef DATASET
#define DATASET

//Print house fonksyonu için biçim öntanımları
#define SINGLE_WITH_TOP 1
#define SINGLE_WITHOUT_TOP 0
#define MULTI 2
#define JUST_TOP 3
#define LIMITLESS 0

//Kriterler
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
#define QUALTHREE 12

//Artan veya Azalan olarak sıralamak için hardcoded değerler
#define ASC 0
#define DESC 1

//Limit_houses için sıralama yok değeri
#define NON -1

//Dosya Türleri
#define TEST 1
#define TRAIN 0

//Liste boyutları
#define HASH_TABLE_SIZE 100
#define HASH_TABLE_SIZE_TYPE_NEIGHBOR ('Z' - 'A') * 2
#define LINE_BUFFER_SIZE 1024

//Renkler
#define RESET   "\x1b[0m"
#define BLUE    "\x1b[94m"
#define CYAN    "\x1b[96m" 
#define RED     "\x1b[91m"
#define GREEN   "\x1b[92m" 
#define MAGENTA "\x1b[95m"
#define YELLOW  "\x1b[93m"

//Ev yapısı
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

//Hash table
void create_hash_table(House * house_list_head, House * houses[], int hash_type);
House* linearise_hash_table (House * ht[], int hash_type, int * lenght);

//Liste manülpasyonu
void limit_houses(House** houses_head, int criter_name, int min, int max, int * new_lenght);
void sort_houses(House** houses, int criter_name, int order);

//Sıralama
void merge_sort(House ** list, int criter_name, int order);
void split_list(House* input, House** first_half, House** second_half);
House* merge(House* in1, House* in2, int criter_name, int order);
House** pull_from_table_by_id_as_headref(int id, House* house_list[]);

//Girdi - Çıktı
void print_house(House * house, int style, int limit);

//Dosya Okuma - Yazma
int read_house_from_file(char* filename, House * hById[], House * hByN[], int file_type);
void write_house_to_file(House* head, char* filename, int limit);

//Veri döndürme - dönüştürme
char * ghc_s (House * house, int criter_name);
int ghc_i (House * house, int criter_name);
House * ghc_p (House * house, int criter_name);

int convert_kitchenqual (char * c);
char * convert_kitchenqual_back (int value);

House* get_neighborhoods(House * house, House * houses[], int * lenght);
House* get_house_byid(int id, House * houses[]);

int get_criter_avg_of_house(House* head, int criter);


//Diğer
void mean_sale_prices (House * ht[], int criter);
void create_hash_table_tree(House * houses[], int hash_type);

#endif
