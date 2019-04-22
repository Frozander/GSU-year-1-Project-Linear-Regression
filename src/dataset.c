#include"dataset.h"
#include<stdio.h>
#include<stdlib.h>

void read_house_data(char* filename, House houses[]){
  printf("Reading file %s\n",filename);  
  // TODO

  FILE *openFile = fopen(filename, "r");
  char readLine[128];

  fgets(readLine, sizeof(readLine), openFile);


  int type;
  // Tam emin olamadım o yüzden böyle bir şey denedim
  // Type == 1 -> Train   Type == 0 -> Test  Type == -1 -> Error
  if(0 == strcmp(readLine, "Id,LotArea,Street,SalePrice,Neighborhood,YearBuilt,OverallQual,OverallCond,KitchenQual")){
    type = 1;
  } else if (0 == strcmp(readLine, "Id,LotArea,Street,Neighborhood,YearBuilt,OverallQual,OverallCond,KitchenQual"))
  {
    type = 0;
  } else type = -1;
  

  switch (type)
  {
    case 0:
      // Test
      for(size_t i = 0; !feof(openFile); i++)
      {
        fscanf(openFile, "%d,%d,%s,%d,%s,%d,%d,%d,%d\n", houses[i].id, houses[i].kitchenqual, houses[i].lotarea, houses[i].neighborhood, houses[i].overallcond, houses[i].overallqual, houses[i].saleprice, houses[i].street, houses[i].yearbuilt);
      }

      break;
    
    case 1:
      // Train
      for(size_t i = 0; !feof(openFile); i++)
      {
        fscanf(openFile, "%d,%d,%s,%s,%d,%d,%d,%d\n", houses[i].id, houses[i].kitchenqual, houses[i].lotarea, houses[i].neighborhood, houses[i].overallcond, houses[i].overallqual, houses[i].street, houses[i].yearbuilt);
      }

      break;
  
    default:
      // Error
      // TODO: THROW EXCEPTION
      break;
  }



  // TODO: DELETE THIS AFTER IMPLEMENTING THE SWITCH CASE
  for(size_t i = 0; !feof(openFile); i++)
  {
  
    // Possible problems:
    //              not included in both   Not an int
    //                         v               v
    fscanf(openFile, "%d,%d,%s,%d,%s,%d,%d,%d,%d\n", houses[i].id, houses[i].kitchenqual, houses[i].lotarea, houses[i].neighborhood, houses[i].overallcond, houses[i].overallqual, houses[i].saleprice, houses[i].street, houses[i].yearbuilt);


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
