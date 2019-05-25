#include"dataset.h"
#include"models.h"

#define CSV_TEST_PREDEFINED "../data/data_test.csv"
#define CSV_TRAIN_PREDEFINED "../data/data_train.csv"
#define CSV_LINEER_OUT_PREDEFINED "../out/lineer_out.csv"
#define CSV_RELATIONAL_OUT_PREDEFINED "../out/relational_out.csv"

#define ANA_MENU 0
#define KRITERE_GORE_LISTELE 1
#define ID_VERILEN_EVI_BUL 2
#define KOMSU_EVLERI_LISTELE 3
#define GRUPLANDIRARAK_GOSTER 4
#define FIYAT_TAHMINI_YAP 5
#define GELISTIRICI_SECENEKLERI 6

#define ID_AL 1
#define BASILACAK_SAYISI_BELIRLE 2
#define SIRALAMA_YONU_SEC 3
#define KRITER_SEC 4
#define KRITER_EKLENSIN_MI 5
#define SONUC_GORUNTULEME_BICIMI_SEC 6
#define TEKRAR_YAP_SOR 7
#define TAHMIN_YONTEMI_SEC 8
#define EXIT -1
#define DOGRUSALLIK_YONTEMI 9
#define BENZERLIK_YONTEMI 10
#define KULLANILACAK_VERIYI_SEC 11
#define KULLANILACAK_VERININ_GRUBUNU_SEC 12
#define DOSYA_ADRESI_AL 13

#define TEST_VERISI 1
#define OGRENME_VERISI 2

#define DOSYAYA_YAZDIR 1
#define EKRANA_BASTIR 2


#define HASH_TABLE_SIZE 100
#define LINE 100

#define RESET   "\x1b[0m"
#define BLUE    "\x1b[94m"
#define CYAN    "\x1b[96m" 
#define RED     "\x1b[91m"
#define GREEN   "\x1b[92m" 
#define MAGENTA "\x1b[95m"
#define YELLOW  "\x1b[93m"

void slice_str(char * str, char * buffer, int start, int end);
int check_extention (char * c, char * ext);
int check_csv(char * c);
int r_int ();
char r_char ();

char csv_test_data_directory[LINE];
char csv_train_data_directory[LINE];
char csv_out_directory[LINE];


House* housesById[HASH_TABLE_SIZE];
House* housesByNeighbor[HASH_TABLE_SIZE];

House* housesById_test[HASH_TABLE_SIZE];
House* housesByNeighbor_test[HASH_TABLE_SIZE];

//For printing the sorted list to output
House * tmp_head;

int main(int argc,char * argv[]){

  //on tanimli degerler ataniyor
  strcpy(csv_train_data_directory, CSV_TRAIN_PREDEFINED);
  strcpy(csv_test_data_directory, CSV_TEST_PREDEFINED);

  printf("\x1B[2J"); //konsol ekrani temizleniyor

  //main icinde kullanilan degiskenler tanimlaniyor
  int menu = 0;
  int sub_menu = 0;
  int secim_2 = 0;
  int secim_3 = 0;
  int kriter = 0;
  int basilacak_sayisi = 0;
  int verin_bulundugu_grup = 0;
  int kullanilacak_veri = 0;
  int goruntuleme_bicimi = 0;
  int yontem = 0;
  int secim_id = 0;
  char secim_c;
  char secim_c_2;
  char secim_c_3;
  House * tmp_h;

  if(argc < 2 || argv[1] == NULL) {
    printf(CYAN "\nOgrenme verisi program cagirilirken arguman olarak girilmemis, ne yapmak istersiniz?\n\n");
    printf(BLUE "1 - On tanimli belge adresini kullan (%s)\n", CSV_TRAIN_PREDEFINED);
    printf(GREEN "2 - Ogrenme verisinin adresini gir\n");
    printf(RED "3 - Programi kapat\n\n");
    printf(MAGENTA "Seciminiz : " RESET);
    menu = r_int();
    if(menu == 1) {
      strcpy(csv_train_data_directory, CSV_TRAIN_PREDEFINED);
      printf(GREEN "\n\nOgrenme verisi ontanimli ayar secildi : " CYAN "%s\n\n" RESET, csv_train_data_directory);
    } else if (menu == 2) {
      printf(MAGENTA "\nOgrenme verisi adresi : " RESET);
      scanf("%s", csv_train_data_directory);
      if(check_csv(csv_test_data_directory)) {
        printf(GREEN "\n\nOgrenme verisi adresi alindi : " CYAN "%s\n\n" RESET, csv_train_data_directory);
      } else {
        printf(RED "\n\nAdres bir csv dosyasini gostermiyor\n\n" RESET);
        strcpy(csv_train_data_directory, CSV_TRAIN_PREDEFINED);
        printf(GREEN "\nOgrenme verisi otomatik olarak on tanimli ayar secildi : " CYAN "%s\n\n" RESET, csv_train_data_directory);
      }
    } else if ( menu == 3) {
      printf(RED "\n\nProgramdan cikiliyor\n\n" RESET);
      return 0;
    }else {
      printf(RED "\nTanimsiz menu!\n" RESET);
      strcpy(csv_train_data_directory, CSV_TRAIN_PREDEFINED);
      printf(GREEN "\nOgrenme verisi otomatik olarak on tanimli ayar secildi : " CYAN "%s\n\n" RESET, csv_train_data_directory);
    }
  } else {
    strcpy(csv_train_data_directory, argv[1]);
    printf(GREEN "\n\nOgrenme verisi arguman olarak alindi : " CYAN "%s\n\n" RESET, csv_train_data_directory);
  }
  
  menu = 0;
  if(argc < 3 || argv[2] == NULL) {
    printf(CYAN "\nTest verisi program cagirilirken arguman olarak girilmemis, ne yapmak istersiniz?\n\n");
    printf(BLUE "1 - On tanimli belge adresini kullan (%s)\n", CSV_TEST_PREDEFINED);
    printf(GREEN "2 - Test verisinin adresini gir\n");
    printf(RED "3 - Programi kapat\n\n");
    printf(MAGENTA "Seciminiz : " RESET);
    menu = r_int();
    if(menu == 1) {
      strcpy(csv_test_data_directory, CSV_TEST_PREDEFINED);
      printf(GREEN "\n\nTest verisi ontanimli ayar secildi : " CYAN "%s\n\n" RESET, csv_test_data_directory);
    } else if (menu == 2) {
      printf(MAGENTA "\nTest verisi adresi : " RESET);
      scanf("%s", csv_test_data_directory);
      if(check_csv(csv_test_data_directory)) {
        printf(GREEN "\n\nTest verisi adresi alindi : " CYAN "%s\n\n" RESET, csv_test_data_directory);
      } else {
        printf(RED "\n\nAdres bir csv dosyasini gostermiyor\n\n" RESET);
        strcpy(csv_test_data_directory, CSV_TEST_PREDEFINED);
        printf(GREEN "\nTest verisi otomatik olarak on tanimli ayar secildi : " CYAN "%s\n\n" RESET, csv_test_data_directory);
      }
    } else if ( menu == 3) {
      printf(RED "\n\nProgramdan cikiliyor\n\n" RESET);
      return 0;
    }else {
      printf(RED "\nTanimsiz menu!\n" RESET);
      strcpy(csv_test_data_directory, CSV_TEST_PREDEFINED);
      printf(GREEN "\nTest verisi otomatik olarak on tanimli ayar secildi : " CYAN "%s\n\n" RESET, csv_test_data_directory);
    }
  } else {
    strcpy(csv_test_data_directory, argv[2]);
    printf(GREEN "\n\nTest verisi arguman olarak alindi : " CYAN "%s\n\n" RESET, csv_test_data_directory);
  }

  //Ev verilerinin belgelerden okunmasi
  read_house_data(csv_train_data_directory, housesById, housesByNeighbor, TRAIN);
  read_house_data(csv_test_data_directory, housesById_test, housesByNeighbor_test, TEST);
  
  menu = ANA_MENU;
  
  while(menu != EXIT){
    
    if (menu == ANA_MENU) {
      printf(GREEN "\n\nANA MENU\n");
      printf(CYAN "\nHangi islemi yapmak istiyorsunuz?\n\n");
      printf(BLUE "1 - Kriter"CYAN"(ler)"BLUE"e gore evleri limitle, sırala ve listele " MAGENTA "(ekrana bas / dosyaya kaydet)\n");
      printf(BLUE "2 - ID degeri verilen evi bastir " MAGENTA " (ekrana bas)\n");
      printf(BLUE "3 - ID degeri verilen evin komsu evlerini listele " MAGENTA " (ekrana bas / dosyaya kaydet)\n");
      printf(BLUE "4 - Kritere gore ortalama fiyatlari gruplandirarak goster " MAGENTA " (ekrana bas)\n");
      printf(BLUE "5 - Fiyat tahmini yap\n");
      printf(BLUE "6 - Gelistirici Secenekleri\n");
      printf(RED "0 - Programi kapat\n");
      printf(MAGENTA "\nSeciminiz: " RESET);
      menu = r_int();
      if(menu == 0) menu = EXIT;
    } else if (menu == KRITERE_GORE_LISTELE) {
      printf(GREEN "\n\nKRITERE GORE EV LISTELEME\n");

      //fonksyonda kullanilacak degiskenler baslangic degerlerine donduruluyor
      tmp_h = linearise_hash_table(housesById, ID);
      sub_menu = KRITER_EKLENSIN_MI;

      while( sub_menu != EXIT) {
        if( sub_menu == KRITER_EKLENSIN_MI) {
          printf(CYAN "\nKriter filtiresi eklemek istiyor mususunuz?\n");
          printf(BLUE "\n1 - Evet\n");
          printf(BLUE "2 - Hayir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();
          if (secim_2 == 1) {
            sub_menu = KRITER_SEC;
          } else if ( secim_2 == 2) {
            sub_menu = SIRALAMA_YONU_SEC;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = KRITER_EKLENSIN_MI;
          }
        } else if(sub_menu == KRITER_SEC) {
          printf(CYAN "\nHangi kritere gore filtirelensin\n");
          printf(BLUE "\n1 - LOTAREA \n2 - STREET \n3 - SALEPRICE\n4 - NEIGHBORHOOD\n5 - YEARBUILT\n6 - OVERALLQUAL\n7 - OVERALLCOND\n8 - KITCHENQUAL\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          kriter = r_int();
          if( kriter == 1 || kriter == 3 || kriter == 5 || kriter == 6 || kriter == 7 || kriter == 8) { //Int tipi kriterler
            printf(CYAN "\nAlt limit nedir (atlamak icin sifirdan kucuk deger girin): " RESET);
            secim_2 = r_int();
            secim_2 = (secim_2 < 0) ? -1 : secim_2;
            printf(CYAN "\n\nUst limit nedir (atlamak icin sifirdan kucuk deger girin): " RESET);
            secim_3 = r_int();
            secim_3 = (secim_3 < 0) ? -1 : secim_3;
            limit_houses(&tmp_h, kriter, secim_2, secim_3);
            printf(GREEN "\nListe %d - %d araliginda limitlendi\n" RESET, secim_2, secim_3);
            sub_menu = KRITER_EKLENSIN_MI;
          } else if (kriter == 2 || kriter == 4) { //String tipi kriterler
            printf(CYAN "\nAlt limit karakter nedir (A-Z) (atlamak icin aralik disi deger girin): " RESET);
            secim_c_2 = r_char();
            secim_c_2 = (secim_c_2 < 'A' || secim_c_2 > 'Z') ? -1 : secim_c_2;
            printf(CYAN "\n\nUst limit karakter nedir (A-Z) (atlamak icin aralik disi deger girin): " RESET);
            secim_c_3 = r_char();
            secim_c_3 = (secim_c_3 < 'A' || secim_c_3 > 'Z') ? -1 : secim_c_3;
            limit_houses(&tmp_h, kriter, secim_2, secim_3);
            printf(GREEN "\nListe %c - %c araliginda limitlendi\n" RESET, secim_c_2, secim_c_3);
            sub_menu = KRITER_EKLENSIN_MI;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = KRITER_SEC;
          }
        } else if( sub_menu == SIRALAMA_YONU_SEC) {
          printf(CYAN "\nHangi yonde sirali bastirmak istiyorsunuz?\n");
          printf(BLUE "\n1 - Artan\n");
          printf(BLUE "2 - Azalan\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();
          if (secim_2 == 1) {
            sort_houses(&tmp_h, kriter, ASC);
            sub_menu = BASILACAK_SAYISI_BELIRLE;
          } else if ( secim_2 == 2) {
            sort_houses(&tmp_h, kriter, DESC);
            sub_menu = BASILACAK_SAYISI_BELIRLE;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = SIRALAMA_YONU_SEC;
          }
        } else if( sub_menu == BASILACAK_SAYISI_BELIRLE) {
          printf(CYAN "\nBasilacak sonuc sayisi (0 yazarak sinir koymadan basabilirsiniz): " RESET);
          basilacak_sayisi = r_int();
          if( basilacak_sayisi < 0) {
            printf(CYAN "\nBasilacak sonuc sayisi sifirdan kucuk olamaz\n" RESET);
            sub_menu = BASILACAK_SAYISI_BELIRLE;
          } else {
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          }
        } else if( sub_menu == SONUC_GORUNTULEME_BICIMI_SEC) {
          printf(CYAN "\nSonuclari ne sekilde goruntulemek istiyorsunuz?\n");
          printf(BLUE "\n1 - Dosyaya yazdir\n");
          printf(BLUE "2 - Ekrana bastir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();
          if (secim_2 == 1) {
            //dosyaya yazdir
            //file_write_house(&tmp_head, "../out/sortedhouses.csv");
            sub_menu = EXIT;
          } else if (secim_2 == 2) {
            print_house(tmp_h, MULTI, basilacak_sayisi);
            sub_menu = EXIT;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          }
        } else {
          printf(RED "\nHATA : Tanimsiz alt menu - Ana menuye donuluyor\n" RESET);
          sub_menu = EXIT;
          menu = ANA_MENU;
        }
      }
      menu = ANA_MENU;
    } else if ( menu == ID_VERILEN_EVI_BUL){
      printf(GREEN "\n\nID DEGERI VERILEN EVI BASTIRMA\n");

      //fonksyonda kullanilacak degiskenler baslangic degerlerine donduruluyor
      sub_menu = ID_AL;
      
      while( sub_menu != EXIT) {
        if( sub_menu == ID_AL) {
          printf(CYAN "\nBastirmak istediginiz evin ID si: "RESET);
          secim_id = r_int();
          sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC;
        } else if( sub_menu == KULLANILACAK_VERININ_GRUBUNU_SEC) {
          printf(CYAN "\nBastirmak istediginiz ev hangi veri grubunda? \n");
          printf(BLUE "\n1 - Ogrenme Verileri\n");
          printf(BLUE "2 - Test Verileri\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();

          if(secim_2 == 1) {
            print_house(get_house_byid(secim_id, housesById), SINGLE_WITH_TOP, LIMITLESS);
            sub_menu = TEKRAR_YAP_SOR;
          } else if (secim_2 == 2) {
            print_house(get_house_byid(secim_id, housesById_test), SINGLE_WITH_TOP, LIMITLESS);
            sub_menu = TEKRAR_YAP_SOR;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC;
          }
          
        } else if( sub_menu == TEKRAR_YAP_SOR) {
          printf(CYAN "\nBaska bir ev daha bastirmak ister misiniz? \n");
          printf(BLUE "\n1 - Evet\n");
          printf(BLUE "2 - Hayir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();

          if(secim_2 == 1) {
            sub_menu = ID_AL; //basa don
          } else if (secim_2 == 2) {
            sub_menu = EXIT; //fonksyondan cik
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = TEKRAR_YAP_SOR; //tekrar sor
          }

        } else {
          printf(RED "\nHATA : Tanimsiz alt menu - Ana menuye donuluyor\n" RESET);
          sub_menu = EXIT;
          menu = ANA_MENU;
        }
      }
      menu = ANA_MENU;
    } else if ( menu == KOMSU_EVLERI_LISTELE){
      printf(GREEN "\n\nID DEGERI VERILEN EVIN KOMSULARINI LISTELEME\n");

      //fonksyonda kullanilacak degiskenler baslangic degerlerine donduruluyor
      sub_menu = ID_AL;
      
      while( sub_menu != EXIT) {
        if( sub_menu == ID_AL) {
          printf(CYAN "\nKomsularini listelemek istediginiz evin ID si: "RESET);
          secim_id = r_int();
          sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC;
        } else if( sub_menu == KULLANILACAK_VERININ_GRUBUNU_SEC) {
          printf(CYAN "\nBu ev hangi veri grubunda? \n");
          printf(BLUE "\n1 - Ogrenme Verileri\n");
          printf(BLUE "2 - Test Verileri\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();

          if(secim_2 == 1) {
            tmp_h = get_neighborhoods(get_house_byid(secim_id, housesById), housesByNeighbor);
            sub_menu = BASILACAK_SAYISI_BELIRLE;
          } else if (secim_2 == 2) {
            tmp_h = get_neighborhoods(get_house_byid(secim_id, housesById_test), housesByNeighbor);
            sub_menu = BASILACAK_SAYISI_BELIRLE;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC;
          } 
        } else if( sub_menu == BASILACAK_SAYISI_BELIRLE) {
          printf(CYAN "\nBasilacak maksimum sonuc sayisi (0 yazarak sinir koymadan basabilirsiniz): " RESET);
          basilacak_sayisi = r_int();
          if( basilacak_sayisi < 0) {
            printf(CYAN "\nBasilacak maksimum sonuc sayisi sifirdan kucuk olamaz\n" RESET);
            sub_menu = BASILACAK_SAYISI_BELIRLE;
          } else {
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          }
        } else if( sub_menu == SONUC_GORUNTULEME_BICIMI_SEC) {
          printf(CYAN "\nSonuclari ne sekilde goruntulemek istiyorsunuz?\n");
          printf(BLUE "\n1 - Dosyaya yazdir\n");
          printf(BLUE "2 - Ekrana bastir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();
          if (secim_2 == 1) {
            //dosyaya yazdir
            sub_menu = TEKRAR_YAP_SOR;
          } else if (secim_2 == 2) {
            print_house(tmp_h, MULTI, basilacak_sayisi);
            sub_menu = TEKRAR_YAP_SOR;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          }
        } else if( sub_menu == TEKRAR_YAP_SOR) {
          printf(CYAN "\nBaska bir evin daha komsularini listelemek ister misiniz? \n");
          printf(BLUE "\n1 - Evet\n");
          printf(BLUE "2 - Hayir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();

          if(secim_2 == 1) {
            sub_menu = ID_AL; //basa don
          } else if (secim_2 == 2) {
            sub_menu = -1; //fonksyondan cik
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = 5; //tekrar sor
          }

        } else {
          printf(RED "\nTanimsiz secim\n" RESET);
          sub_menu = 0; //tekrar sor
        }
      }
      menu = ANA_MENU;
    } else if( menu == GRUPLANDIRARAK_GOSTER){
      printf(GREEN "\n\nKRITERE GORE GRUPLANDIRARAK ORTALAMA FIYATLARI LISTELEME\n");

      //fonksyonda kullanilacak degiskenler baslangic degerlerine donduruluyor
      tmp_h = linearise_hash_table (housesById, ID);
      
      while( sub_menu != -1) {
        if( sub_menu == 0) {
          printf(CYAN "\nHangi kritere gore gruplandirilsin\n");
          printf(BLUE "\n1 - LOTAREA \n2 - STREET \n4 - NEIGHBORHOOD\n5 - YEARBUILT\n6 - OVERALLQUAL\n7 - OVERALLCOND\n8 - KITCHENQUAL\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          kriter = r_int();
          if( kriter >= 1 || kriter == 5 || kriter == 6 || kriter == 7 || kriter == 8 || kriter == 2 || kriter == 4) {
            sub_menu = 2;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = 0;
          }
        } else if( sub_menu == 2) {
          printf(CYAN "\nSonuclari ne sekilde goruntulemek istiyorsunuz?\n");
          printf(BLUE "\n1 - Dosyaya yazdir\n");
          printf(BLUE "2 - Ekrana bastir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();
          if (secim_2 == 1) {
            //dosyaya yazdir
            sub_menu = 3;
          } else if (secim_2 == 2) {
            mean_sale_prices(tmp_head, kriter, 0 );
            sub_menu = 3;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = 2;
          }
        } else if( sub_menu == 3) {
          printf(CYAN "\nBaska bir ev daha bastirmak ister misiniz? \n");
          printf(BLUE "\n1 - Evet\n");
          printf(BLUE "2 - Hayir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();

          if(secim_2 == 1) {
            sub_menu = 0; //basa don
          } else if (secim_2 == 2) {
            sub_menu = -1; //fonksyondan cik
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = 3; //tekrar sor
          }

        } else {
          printf(RED "\nTanimsiz secim\n" RESET);
          sub_menu = 0; //tekrar sor
        }
      }
    } else if( menu == FIYAT_TAHMINI_YAP){
      printf(GREEN "\n\nFIYAT TAHMINI YAPMA\n");

      //fonksyonda kullanilacak degiskenler baslangic degerlerine donduruluyor
      sub_menu = TAHMIN_YONTEMI_SEC;
      
      while( sub_menu != EXIT) {
        if( sub_menu == TAHMIN_YONTEMI_SEC) {
          printf(CYAN "\nHangi yontemle tahmin yapmak istersiniz? \n");
          printf(BLUE "\n1 - Evlerin ozelliklerine gore " MAGENTA "(id / csv)\n");
          printf(BLUE "2 - Dogrusal bir iliski kurarak " MAGENTA" (csv)\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();

          if(secim_2 == 1) {
            yontem = BENZERLIK_YONTEMI;
            sub_menu = KULLANILACAK_VERIYI_SEC;
          } else if (secim_2 == 2) {
            yontem = DOGRUSALLIK_YONTEMI;
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = TAHMIN_YONTEMI_SEC;
          } 
        }else if( sub_menu == KULLANILACAK_VERIYI_SEC) {
          printf(CYAN "\nHani verinin fiyat tahminini yapmak istersiniz \n");
          printf(BLUE "\n1 - ID si verilen evin\n");
          printf(BLUE "2 - Tum test verisinin\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();

          if(secim_2 == 1) {
            kullanilacak_veri = ID;
            sub_menu = ID_AL;
          } else if (secim_2 == 2) {
            kullanilacak_veri = TEST_VERISI;
            sub_menu = BENZERLIK_YONTEMI;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = KULLANILACAK_VERIYI_SEC;
          } 
        }else if( sub_menu == ID_AL) {
          printf(CYAN "\nFiyat tahmini yapmak istediginiz evin ID si: "RESET);
          secim_id = r_int();
          sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC;
        } else if ( sub_menu == KULLANILACAK_VERININ_GRUBUNU_SEC) {
          printf(CYAN "\nBu ev hangi veri grubunda? \n");
          printf(BLUE "\n1 - Ogrenme Verileri\n");
          printf(BLUE "2 - Test Verileri\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();
          if (secim_2 == 1) {
            verin_bulundugu_grup = OGRENME_VERISI;
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          } else if (secim_2 == 2) {
            verin_bulundugu_grup = TEST_VERISI;
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC;
          }
        } else if( sub_menu == SONUC_GORUNTULEME_BICIMI_SEC) {
          printf(CYAN "\nSonuclari ne sekilde goruntulemek istiyorsunuz?\n");
          printf(BLUE "\n1 - Dosyaya yazdir\n");
          printf(BLUE "2 - Ekrana bastir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();
          if (secim_2 == 1) {
            goruntuleme_bicimi = DOSYAYA_YAZDIR;
            sub_menu = DOSYA_ADRESI_AL;
          } else if (secim_2 == 2) {
            goruntuleme_bicimi = EKRANA_BASTIR;
            sub_menu = yontem;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          }
        } else if( sub_menu == DOSYA_ADRESI_AL) {
          printf(CYAN "\nBastirilacak dosyanin adresini nasil belirlemek istersiniz?\n");
          printf(BLUE "\n1 - On tanimli adresi kullan (%s)\n", (yontem == DOGRUSALLIK_YONTEMI) ? CSV_LINEER_OUT_PREDEFINED : CSV_RELATIONAL_OUT_PREDEFINED);
          printf(BLUE "2 - Elle gir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();
          if (secim_2 == 1) {
            strcpy(csv_out_directory, (yontem == DOGRUSALLIK_YONTEMI) ? CSV_LINEER_OUT_PREDEFINED : CSV_RELATIONAL_OUT_PREDEFINED);
            sub_menu = yontem;
          } else if (secim_2 == 2) {
            printf(MAGENTA "\nBastırma adresi : " RESET);
            scanf("%s", csv_out_directory);
            if (check_csv(csv_out_directory)) {
              sub_menu = yontem;
            } else {
              printf(RED "\nGirilen adres bir csv dosyasi degil\n" RESET);
              sub_menu = DOSYA_ADRESI_AL;
            }            
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = DOSYA_ADRESI_AL;
          }
        } else if ( sub_menu == DOGRUSALLIK_YONTEMI ) {
          
        } else if( sub_menu == TEKRAR_YAP_SOR) {
          printf(CYAN "\nBaska bir evin daha komsularini listelemek ister misiniz? \n");
          printf(BLUE "\n1 - Evet\n");
          printf(BLUE "2 - Hayir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          secim_2 = r_int();

          if(secim_2 == 1) {
            menu = FIYAT_TAHMINI_YAP; //basa don
            sub_menu = EXIT;
          } else if (secim_2 == 2) {
            menu = FIYAT_TAHMINI_YAP; //basa don
            sub_menu = EXIT;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = 5; //tekrar sor
          }

        } else {
          printf(RED "\nTanimsiz secim\n" RESET);
          sub_menu = 0; //tekrar sor
        }
      }
    } else if( menu == GELISTIRICI_SECENEKLERI){
      printf("ID Icin Hash Tablosu\n");
      create_hash_table_tree(housesById, ID);
      printf("NEIGHBORORHOODS Icin Hash Tablosu\n");
      create_hash_table_tree(housesByNeighbor, NEIGHBORHOOD);
    } else {
      printf(RED "\nTanimsiz secim\n" RESET);
      menu = ANA_MENU; //tekrar sor
    }
    if (menu == ANA_MENU) {
      printf(RED "\n\nIslev sonlandiriliyor\n\n" RESET);
    }
  }
  printf(RED "\n\nProgramdan cikiliyor\n\n" RESET);
  return 0; 
}

//int okur
int r_int () {
  int read;
  if(!scanf("%d",&read)==1) scanf("%*[^0-9] %d",&read);
  return read;  
}

//char okur
char r_char () {
  char read;
  scanf("%[a–Z]c", &read);
  return read;
}

int check_csv  (char * c) {
  return check_extention (c, ".csv");
}

int check_extention (char * c, char * ext) {
  int ext_lenght = strlen(ext);
  char buffer[ext_lenght];
  int lenght = strlen(c);
  slice_str(c, buffer, lenght - ext_lenght, lenght);
  return (strcmp(buffer, ext) == 0) ? 1 : 0;
}

//bir stringin belli bir kısmını döndürür
void slice_str(char * str, char * buffer, int start, int end)
{
    int j = 0;
    for ( int i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
}