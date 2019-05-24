#include"dataset.h"
#include"models.h"

#define CSV_TEST_PREDEFINED "../data/data_test.csv"
#define CSV_TRAIN_PREDEFINED "../data/data_train.csv"

#define KRITERE_GORE_LISTELE 1
#define ID_VERILEN_EVI_BUL 2

#define HASH_TABLE_SIZE 100
#define LINE 100

#define RESET   "\x1b[0m"
#define BLUE    "\x1b[94m"
#define CYAN    "\x1b[96m" 
#define RED     "\x1b[91m"
#define GREEN   "\x1b[92m" 
#define MAGENTA "\x1b[95m"
#define YELLOW  "\x1b[93m"

void slice_str(char * str, char * buffer, size_t start, size_t end);
int r_int ();
char r_char ();

char csv_test_data_directory[LINE];
char csv_train_data_directory[LINE];


House* housesById[HASH_TABLE_SIZE];
House* housesByNeighbor[HASH_TABLE_SIZE];

House* housesById_test[HASH_TABLE_SIZE];
House* housesByNeighbor_test[HASH_TABLE_SIZE];

//For printing the sorted list to output
House * tmp_head;

int main(int argc,char * argv[]){

  strcpy(csv_train_data_directory, CSV_TRAIN_PREDEFINED);
  strcpy(csv_test_data_directory, CSV_TEST_PREDEFINED);

  printf("\x1B[2J"); //konsol ekranını temizler

  int cevap = 0;
  if(argc < 2 || argv[1] == NULL) {
    printf(CYAN "\nOgrenme verisi program cagirilirken arguman olarak girilmemis, ne yapmak istersiniz?\n\n");
    printf(BLUE "1 - On tanimli belge adresini kullan (%s)\n", CSV_TRAIN_PREDEFINED);
    printf(GREEN "2 - Ogrenme verisinin adresini gir\n");
    printf(RED "3 - Programi kapat\n\n");
    printf(MAGENTA "Seciminiz : " RESET);
    cevap = r_int();
    if(cevap == 1) {
      strcpy(csv_train_data_directory, CSV_TRAIN_PREDEFINED);
      printf(GREEN "\n\nOgrenme verisi ontanimli ayar secildi : " CYAN "%s\n\n" RESET, csv_train_data_directory);
    } else if (cevap == 2) {
      printf(MAGENTA "\nOgrenme verisi adresi : " RESET);
      scanf("%s", csv_train_data_directory);
      printf(GREEN "\n\nOgrenme verisi arguman olarak alindi : " CYAN "%s\n\n" RESET, csv_train_data_directory);
    } else if ( cevap == 3) {
      printf(RED "\n\nProgramdan cikiliyor\n\n" RESET);
      return 0;
    }else {
      printf(RED "\nTanimsiz cevap!\n" RESET);
      strcpy(csv_train_data_directory, CSV_TRAIN_PREDEFINED);
      printf(GREEN "\nOgrenme verisi otomatik olarak on tanimli ayar secildi : " CYAN "%s\n\n" RESET, csv_train_data_directory);
    }
  }
  
  cevap = 0;
  if(argc < 3 || argv[2] == NULL) {
    printf(CYAN "\nTest verisi program cagirilirken arguman olarak girilmemis, ne yapmak istersiniz?\n\n");
    printf(BLUE "1 - On tanimli belge adresini kullan (%s)\n", CSV_TEST_PREDEFINED);
    printf(GREEN "2 - Test verisinin adresini gir\n");
    printf(RED "3 - Programi kapat\n\n");
    printf(MAGENTA "Seciminiz : " RESET);
    cevap = r_int();
    if(cevap == 1) {
      strcpy(csv_test_data_directory, CSV_TEST_PREDEFINED);
      printf(GREEN "\n\nTest verisi ontanimli ayar secildi : " CYAN "%s\n\n" RESET, csv_test_data_directory);
    } else if (cevap == 2) {
      printf(MAGENTA "\nTest verisi adresi : " RESET);
      scanf("%s", csv_train_data_directory);
      printf(GREEN "\n\nTest verisi arguman olarak alindi : " CYAN "%s\n\n" RESET, csv_test_data_directory);
    } else if ( cevap == 3) {
      printf(RED "\n\nProgramdan cikiliyor\n\n" RESET);
      return 0;
    }else {
      printf(RED "\nTanimsiz cevap!\n" RESET);
      strcpy(csv_test_data_directory, CSV_TEST_PREDEFINED);
      printf(GREEN "\nTest verisi otomatik olarak on tanimli ayar secildi : " CYAN "%s\n\n" RESET, csv_test_data_directory);
    }
  } else {
    printf("%s", argv[2]);
  }

  //Ev verilerinin belgelerden okunmasi
  read_house_data(csv_train_data_directory, housesById, housesByNeighbor, TRAIN);
  read_house_data(csv_test_data_directory, housesById_test, housesByNeighbor_test, TEST);
  
  cevap = 1;
  int secim = 0;
  int secim_2 = 0;
  int secim_3 = 0;
  int secim_kriter = 0;
  int secim_basilacak_sayisi = 0;
  int secim_id = 0;
  char secim_c;
  char secim_c_2;
  char secim_c_3;
  House * tmp_h;
  while(cevap!=0){
    printf(GREEN "\n\nANA MENU\n");
    printf(CYAN "\nHangi islemi yapmak istiyorsunuz?\n\n");
    printf(BLUE "1 - Kriter"CYAN"(ler)"BLUE"e gore evleri limitle, sırala ve listele " MAGENTA "(ekrana bas / dosyaya kaydet)\n");
    printf(BLUE "2 - ID degeri verilen evi bastir " MAGENTA " (ekrana bas)\n");
    printf(BLUE "3 - ID degeri verilen evin komsu evlerini bul " MAGENTA " (ekrana bas)\n");
    printf(BLUE "4 - Kritere gore ortalama fiyatlari gruplandirarak goster " MAGENTA " (ekrana bas)\n");
    printf(BLUE "5 - En yuksek fiyata sahip ilk 10 evi goster " MAGENTA " (ekrana bas)\n");
    printf(BLUE "6 - Sirali ev listesini kaydet\n");
    printf(BLUE "7 - Fiyat tahmini yap\n");
    printf(BLUE "8 - Hash tablosunu ekrana bas\n");
    printf(BLUE "9 - Evleri kritere göre sıralı bastır\n");
    printf(RED "0 - Programi kapat\n");
    printf(MAGENTA "\nSeciminiz: " RESET);
    cevap = r_int();

    if (cevap == KRITERE_GORE_LISTELE) {
      printf(GREEN "\n\nKRITERE GORE EV LISTELEME\n");

      tmp_h = linearise_hash_table(housesById, ID);
      secim = 0;
      secim_kriter = ID;
      secim_basilacak_sayisi = LIMITLESS;

      while (secim != -1) {
        if (secim == 0) {
          printf(CYAN "\nKriter filtiresi eklemek istiyor mususunuz?\n");
          printf(BLUE "\n1 - Evet\n");
          printf(BLUE "2 - Hayir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim = r_int();
        } else if(secim == 1) {
          printf(CYAN "\nHangi kritere gore filtirelensin\n");
          printf(BLUE "\n1 - LOTAREA \n2 - STREET \n4 - NEIGHBORHOOD\n5 - YEARBUILT\n6 - OVERALLQUAL\n7 - OVERALLCOND\n8 - KITCHENQUAL\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_kriter = r_int();
          if( secim_kriter == 1 || secim_kriter == 5 || secim_kriter == 6 || secim_kriter == 7 || secim_kriter == 8) {
            printf(CYAN "\nAlt limit nedir (atlamak icin -1): " RESET);
            secim_2 = r_int();
            printf(CYAN "\n\nUst limit nedir (atlamak icin -1): " RESET);
            secim_3 = r_int();
            limit_houses(&tmp_h, secim_kriter, secim_2, secim_3);
            secim = 0;
          } else if (secim_kriter == 4 || secim_kriter == 5) {
            printf(CYAN "\nAlt limit karakter nedir (atlamak icin -1): " RESET);
            secim_c_2 = r_char();
            printf(CYAN "\n\nUst limit karakter nedir (atlamak icin -1): " RESET);
            secim_c_3 = r_char();
            limit_houses(&tmp_h, secim_kriter, secim_2, secim_3);
            secim = 0;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            secim = 1;
          }
        } else if (secim == 2) {
          printf(CYAN "\nHangi yonde sirali bastirmak istiyorsunuz?\n");
          printf(BLUE "\n1 - Artan\n");
          printf(BLUE "2 - Azalan\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();
          if (secim_2 == 1) {
            sort_houses(&tmp_h, secim_kriter, ASC);
            secim = 3;
          } else if ( secim_2 == 2) {
            sort_houses(&tmp_h, secim_kriter, DESC);
            secim = 3;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            secim = 2;
          }
        } else if (secim == 3) {
          printf(CYAN "\nBasilacak sonuc sayisi (0 yazarak sinir koymadan basabilirsiniz): " RESET);
          secim_basilacak_sayisi = r_int();
          if( secim_basilacak_sayisi < 0) {
            printf(CYAN "\nBasilacak sonuc sayisi sifirdan kucuk olamaz\n" RESET);
            secim = 3;
          } else {
            secim = 4;
          }
        } else if (secim == 4) {
          printf(CYAN "\nSonuclari ne sekilde goruntulemek istiyorsunuz?\n");
          printf(BLUE "\n1 - Dosyaya yazdir\n");
          printf(BLUE "2 - Ekrana bastir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();
          if (secim_2 == 1) {
            //dosyaya yazdir
            secim = -1;
          } else if (secim_2 == 2) {
            print_house(tmp_h, MULTI, secim_basilacak_sayisi);
            secim = -1;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            secim = 4;
          }
        } else {
          printf(RED "\nTanimsiz secim\n" RESET);
          secim = 0;
        }
      }
    }
    else if ( cevap == ID_VERILEN_EVI_BUL){
      printf(GREEN "\n\nID degeri verilen evi bastirma\n");

      secim = 0;
      secim_id = 0;
      
      while( secim != -1) {
        if (secim == 0) {
          printf(CYAN "\nBastirmak istediginiz evin ID si: "RESET);
          secim_id = r_int();
          printf(CYAN "\nBastirmak istediginiz ev hangi veri grubunda? \n");
          printf(BLUE "\n1 - Ogrenme Verileri\n");
          printf(BLUE "2 - Test Verileri\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim = r_int();
        } else if(secim == 1) {
          print_house(get_house_byid(secim_id, housesById), SINGLE_WITH_TOP, LIMITLESS);
          secim = -1;
        } else if (secim == 2) {
          print_house(get_house_byid(secim_id, housesById_test), SINGLE_WITH_TOP, LIMITLESS);
          secim = -1;
        } else {
          printf(RED "\nTanimsiz secim\n" RESET);
          secim = 0;
        }
      }
      
    } else if (cevap==3){
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
        printf("\nSiralanan evler \"../out/sortedhouses.csv\"ye kaydedildi\n");
      } else
      {
        printf("\nEvler siralanmamis!\nOnce 9'u kullanarak evleri siralayin\n");
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
        printf("%s\n Y = %.2lfX + %.2lf", "\nTahmin dogrusunun XY duzlemi uzerinde gosterimi:", W->values[1][0], W->values[0][0]);
        
        Matrix* prediction = make_prediction(&head_test, W);

        //print the output
        matrix_to_house_list(prediction, &head_test);        
        file_write_house(&head_test, "../out/predictions.csv");
        printf("\nTahminler \"../out/predictions.csv\"ye kaydedildi\n");
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
    printf(RED "\n\nSonlandiriliyor\n\n" RESET);
  }

  return 0; 
}

//int okur
int r_int () {
  int read;
  scanf("%d", &read);
  return read;
}

//char okur
char r_char () {
  char read;
  scanf("%c", &read);
  return read;
}

//bir stringin belli bir kısmını döndürür
void slice_str(char * str, char * buffer, size_t start, size_t end)
{
    size_t j = 0;
    for ( size_t i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
}