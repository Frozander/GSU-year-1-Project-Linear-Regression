#include"dataset.h"
#include"models.h"

#define HASH_TABLE_SIZE 100

char * csv_test_data_directory = "../data/data_test.csv";
char * csv_train_data_directory = "../data/data_train.csv";

House* housesById[HASH_TABLE_SIZE];
House* housesByNeighbor[HASH_TABLE_SIZE];

House* housesById_test[HASH_TABLE_SIZE];
House* housesByNeighbor_test[HASH_TABLE_SIZE];

//For printing the sorted list to output
House * tmp_head;

int main(int argc,char * argv[]){

  if(argv[1] != NULL) {
    csv_train_data_directory = argv[1];
  }

  read_house_data(csv_train_data_directory, housesById, housesByNeighbor, TRAIN);
  read_house_data(csv_test_data_directory, housesById_test, housesByNeighbor_test, TEST);
  
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
    printf("8 - Hash tablosunu ekrana bas\n");
    printf("9 - Evleri kritere göre sıralı bastır\n");
    printf("Programdan cikmak icin 0 a basiniz.\n");
    scanf("%d",&cevap);
    if (cevap==1){
      printf("Ev listesi \n");
      House * tmp;
      int counter = housesById[0]->id;
      print_house(tmp, JUST_TOP, LIMITLESS);
      do
      {
        tmp = get_house_byid(counter, housesById);
        print_house(tmp, SINGLE_WITHOUT_TOP, LIMITLESS);
        counter++;
      } while (get_house_byid(counter, housesById) != NULL);
      
    }
    else if (cevap==2){
      printf("ID degeri verilen ev \n");
      int tmp_id = 0;
      printf("ID degeri girin: ");
      scanf("%d", &tmp_id);
      print_house(get_house_byid(tmp_id, housesById), SINGLE_WITH_TOP, LIMITLESS);
    }
    else if (cevap==3){
      printf("Verilen evin komsu degerlerini bulma \n");
      int tmp_id = 0;
      printf("ID degeri girin: ");
      scanf("%d", &tmp_id);
      House * tmp = get_neighborhoods(get_house_byid(tmp_id, housesById), housesByNeighbor);
      print_house(tmp, MULTI, LIMITLESS); 
    }
    else if(cevap==4){
      int c = 0;
      int c_data = 0;
      printf("Verilen kritere gore ortalama ev fiyatlarini bulma\n ");
      printf("Hangi kriter isteniyor?\n");
      printf(" LOTAREA | 1 \n STREET | 2 \n NEIGHBORHOOD | 4\n YEARBUILT | 5\n OVERALLQUAL | 6\n OVERALLCOND | 7\n KITCHENQUAL | 8\n");
      printf("Seciminizi girin: ");
      scanf("%d", &c);

      if(c==LOTAREA || c==STREET || c==NEIGHBORHOOD || c== YEARBUILT || c==OVERALLQUAL || c==OVERALLCOND || c==KITCHENQUAL) {
        if(c == LOTAREA || c==YEARBUILT) {
          printf ("Ortalamalar kaclik araliklara gore belirlensin?\n");
          printf("Aralik degeri girin: ");
          scanf("%d", &c_data);
        } 
        House * head = linearise_hash_table(housesById, ID);
        mean_sale_prices(head, c, c_data);
      }else {
        printf("Hatali bir deger girdiniz\n");
      }
    }
    else if (cevap==5){
      printf("En yuksek fiyatli evler\n");
      printf("Kac ev gormek istersiniz \n");
      int limit = 0;
      printf("Ev sayisi girin: ");
      scanf("%d", &limit);
      House * head = linearise_hash_table(housesById, ID);
      sort_houses(&head, SALEPRICE, DESC);
      print_house(head, MULTI, limit);
    }
    else if (cevap==6){
      if (tmp_head != NULL)
      {
        file_write_house(&tmp_head, "../out/sortedhouses.csv");
        printf("\nSorted houses are printed at ../out/sortedhouses.csv\n");
      } else
      {
        printf("\nHouses are not sorted yet!\nFirst use option 9 to sort the houses\n");
      }
      
            
    }
    else if(cevap==7){
      printf("Fiyat tahmini yap\n");
      int input = 0;
      printf("Yöntem(matrix: 0|ortalama: 1): ");
      scanf("%d", &input);
      House* head = linearise_hash_table(housesById, ID);
      House* head_test = linearise_hash_table(housesById_test, ID);
      sort_houses(&head_test, ID, ASC);
      if (input == 0)
      {
        Matrix* W;
        W = calculate_parameter(head);
        //print_matrix(W);
        printf("%s\n Y = %.2lfX + %.2lf", "\nOutput Values on the XY Plane:", W->values[1][0], W->values[0][0]);
        
        Matrix* prediction = make_prediction(&head_test, W);

        //print the output
        matrix_to_house_list(prediction, &head_test);        
        file_write_house(&head_test, "../out/predictions.csv");
        printf("\nPredictions are printed in ../out/predictions.csv\n");
      } else if (input == 1)
      {
        int tmp_id = 0;
        printf("ID degeri girin: ");
        scanf("%d", &tmp_id);
        House * tmp_h = get_house_byid(tmp_id, housesById);
        printf("Tahmin Edilen:%d\nGerçek Değer:%d\n", model_by_similarity(housesByNeighbor, tmp_h), ghc_i(tmp_h, SALEPRICE));
      } else
      {
        printf("Hatali giris!\n");
      } 
    }
    else if(cevap==8){
      printf("ID Icin Hash Tablosu\n");
      create_hash_table_tree(housesById, ID);
      printf("NEIGHBORORHOODS Icin Hash Tablosu\n");
      create_hash_table_tree(housesByNeighbor, NEIGHBORHOOD);
    }
    else if (cevap == 9) {
      printf("Sirali olarak evleri bastir\n");
      int c = 0;
      printf("Hangi kriter isteniyor?\n");
      printf(" LOTAREA | 1 \n STREET | 2 \n NEIGHBORHOOD | 4\n YEARBUILT | 5\n OVERALLQUAL | 6\n OVERALLCOND | 7\n KITCHENQUAL | 8\n");
      printf("Seciminizi girin: ");
      scanf("%d", &c);

      if(c==LOTAREA || c==STREET || c==NEIGHBORHOOD || c==YEARBUILT || c==OVERALLQUAL || c==OVERALLCOND || c==KITCHENQUAL) {
        tmp_head = linearise_hash_table(housesById, ID);
        sort_houses(&tmp_head, c, ASC);
        print_house(tmp_head, MULTI, LIMITLESS);
      } else {
        printf("Hatali giris");
      }
    }
    printf("Programdan cikiliyor\n");
  }

  return 0; 
}

//test-branch