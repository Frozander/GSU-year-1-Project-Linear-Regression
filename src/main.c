#include"dataset.h"
#include"models.h"

int main(int argc,char * argv[]){
  int cevap;
  while(cevap!=0){
    printf("Emlak Programina Hosgeldiniz!\n");
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
      // TODO
      // print_house ile tek evi goster
      // get_house_byid cagrisi yap
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