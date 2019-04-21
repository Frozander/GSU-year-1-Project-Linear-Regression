#include"dataset.h"
#include<stdio.h>
#include<stdlib.h>

void read_house_data(char* filename, House houses[]){
  printf("Reading file %s\n",filename);  
  // TODO 
  return ; 
}

void print_house(House house){
  printf("Print House dataset\n");
  // TODO 
  return;
}

House get_house_byid(int id){
  printf("Get house with id %d \n",id);
  // TODO 
  return; 
}

House* get_neighborhoods(House house){
  printf("Get neighborhoods of house with id %d\n",house.id);
  // TODO
  return;
}

float* mean_sale_prices(House* houses,char* criter_name){
  printf("Calculate mean sale prices by %s \n",criter_name);
  // TODO
  return ;
}
void sort_houses(House* houses,char* criter_name){
  printf("Sort house by %s and save \n",criter_name);
  // TODO
  return ;
}
