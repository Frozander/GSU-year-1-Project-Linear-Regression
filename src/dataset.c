#include"dataset.h"
#include<stdio.h>
#include<stdlib.h>

void read_house_data(char* filename, House houses[]){
  printf("Reading file %s\n",filename);  
  // TODO

  FILE *openFile = fopen(filename, "r");
  char readLine[128];

  fgets(readLine, sizeof(readLine), openFile);

  // TODO: Ask how we can change the function if there is a missing type
  for(size_t i = 0; !feof(openFile); i++)
  {
  
    // Possible problems:
<<<<<<< HEAD
    //              not included in both   Not an int
=======
    //               not included in both  Not an int
>>>>>>> 0f49a6f25bd561c393c86dff833ddf954778075b
    //                         v               v
    fscanf(openFile, "%d,%d,%s,%d,%s,%d,%d,%d,%d", houses[i].id, houses[i].kitchenqual, houses[i].lotarea, houses[i].neighborhood, houses[i].overallcond, houses[i].overallqual, houses[i].saleprice, houses[i].street, houses[i].yearbuilt);


  }
  
  
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
