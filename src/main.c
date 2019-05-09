#include"dataset.h"
#include"models.h"

#define HASH_TABLE_SIZE 100

char * csv_test_data_directory = "../data/data_test.csv";
char * csv_train_data_directory = "../data/data_train.csv";

House* houses[HASH_TABLE_SIZE];

int main(int argc,char * argv[]){
  read_house_data(csv_train_data_directory, houses);
  create_hash_table_tree(houses);
  int cevap = 1;
  while(cevap!=0){
    printf("\nEmlak Programina Hosgeldiniz!\n");
    printf("Yapmak istediginiz islemi seciniz\n");
    printf("1 - Evleri listele\n");
    printf("2 - ID degeri verilen evi goster\n");
    printf("3 - ID degeri verilen evin komsu evlerini bul \n");
    printf("4 - Semtlere gore satis fiyati ortalamalarini goster\n");
    printf("5 - En yuksek fiyata sahip ilk 10 evi goster\n");
    printf("6 - Sirali ev listesini kaydet\n");
    printf("7 - Fiyat tahmini yap\n");
    printf("Programdan cikmak icin 0 a basiniz.\n");
    scanf("%d",&cevap);
    if (cevap==1){
      printf("Ev listesi \n");
      // TODO
      // Butun evleri listele
     }
    else if (cevap==2){
      printf("ID degeri verilen ev \n");
      int tmp_id = 0;
      printf("ID degeri girin: ");
      scanf("%d", &tmp_id);
      print_house(get_house_byid(tmp_id, houses));
     }
     else if (cevap==3){
      printf("Komsu evler \n");
      // TODO
      // get_neighborhoods 
     }
     else if(cevap==4){
       printf("Ortalama satislar\n");
       // TODO
       // mean_sale_prices
     }
     else if (cevap==5){
       printf("En yuksek fiyatli evler\n");
       // TODO 
     }
     else if (cevap==6){
       printf("Sirali evler\n");
       // TODO 
     }
     else if(cevap==7){
       printf("Fiyat tahmini yap\n");
       // TODO
     }
    printf("Programdan cikiliyor\n");
  }

  return 0; 
}

//test-branch