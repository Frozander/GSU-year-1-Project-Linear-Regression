#include"dataset.h"
#include"models.h"

#define CSV_TEST_PREDEFINED "../data/data_test.csv"
#define CSV_TRAIN_PREDEFINED "../data/data_train.csv"
#define CSV_LINEER_OUT_PREDEFINED "../out/lineer_out.csv"
#define CSV_RELATIONAL_OUT_PREDEFINED "../out/relational_out.csv"
#define CSV_FILTERED_TEST_OUT_PREDEFINED "../out/filtered_test_data_out.csv"
#define CSV_FILTERED_TRAIN_OUT_PREDEFINED "../out/filtered_train_data_out.csv"
#define CSV_LIST_NEIGHBORS_OUT_PREDEFINED "../out/list_neighbors_out.csv"

#define ANA_MENU 0
#define KRITERE_GORE_LISTELE 1
#define ID_VERILEN_EVI_BUL 2
#define KOMSU_EVLERI_LISTELE 3
#define GRUPLANDIRARAK_GOSTER 4
#define FIYAT_TAHMINI_YAP 5
#define GELISTIRICI_SECENEKLERI 6

#define EXIT -1
#define ID_AL 1
#define BASILACAK_SAYISI_BELIRLE 2
#define SIRALAMA_YONU_SEC 3
#define KRITER_SEC 4
#define KRITER_EKLENSIN_MI 5
#define SONUC_GORUNTULEME_BICIMI_SEC 6
#define TEKRAR_YAP_SOR 7
#define TAHMIN_YONTEMI_SEC 8
#define DOGRUSALLIK_YONTEMI 9
#define BENZERLIK_YONTEMI 10
#define KULLANILACAK_VERIYI_SEC 11
#define KULLANILACAK_VERININ_GRUBUNU_SEC 12
#define DOSYA_ADRESI_AL 13
#define TEST_VERISI 14
#define OGRENME_VERISI 15
#define DOSYAYA_YAZDIR 16
#define EKRANA_BASTIR 17

#define LINE 100



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

int main(int argc,char * argv[]){

  //on tanimli degerler ataniyor
  strcpy(csv_train_data_directory, CSV_TRAIN_PREDEFINED);
  strcpy(csv_test_data_directory, CSV_TEST_PREDEFINED);

  printf("\x1B[2J"); //konsol ekrani temizleniyor

  //main icinde kullanilan degiskenler tanimlaniyor
  int menu, sub_menu, mini_menu, min, max, kriter, basilacak_sayisi, verinin_bulundugu_grup, kullanilacak_veri, goruntuleme_bicimi, yontem, id, tahmin, list_lenght, sayac = 0;
  char min_c, max_c = 0;
  House * house_p, * house_p_2;
  Matrix * matrix_p, * matrix_p_2;

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
      sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC;

      while( sub_menu != EXIT) {
        if( sub_menu == KULLANILACAK_VERININ_GRUBUNU_SEC) {
          printf(CYAN "\nListelemek istediginiz evler hangi veri grubunda? \n");
          printf(BLUE "\n1 - Ogrenme Verileri\n");
          printf(BLUE "2 - Test Verileri\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();
          sayac = 0;
          if(mini_menu == 1) {
            house_p = linearise_hash_table(housesById, ID, &list_lenght);
            verinin_bulundugu_grup = OGRENME_VERISI;
            sub_menu = KRITER_EKLENSIN_MI;
            printf(BLUE "\nOgrenme Verileri" GREEN" secildi, bu grupta" MAGENTA " %d " GREEN "ev bulunuyor\n" RESET, list_lenght);
          } else if (mini_menu == 2) {
            house_p = linearise_hash_table(housesById_test, ID, &list_lenght);
            verinin_bulundugu_grup = TEST_VERISI;
            sub_menu = KRITER_EKLENSIN_MI;
            printf(BLUE "\nTest Verileri" GREEN" secildi, bu grupta" MAGENTA " %d " GREEN "ev bulunuyor\n" RESET, list_lenght);
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC;
          }
          
        }
        else if( sub_menu == KRITER_EKLENSIN_MI) {
          printf((sayac > 0) ? CYAN "\nBu filtirelemenin ustune bir tane daha filtireleme yapmak istiyor musunuz?\n" : CYAN "\nBir kriter filtiresi eklemek istiyor musunuz?\n");
          printf(BLUE "\n1 - Evet\n");
          printf(BLUE "2 - Hayir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();
          if (mini_menu == 1) {
            sub_menu = KRITER_SEC;
            sayac++;
          } else if ( mini_menu == 2) {
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
            min = r_int();
            min = (min < 0) ? -1 : min;
            printf(CYAN "\nUst limit nedir (atlamak icin sifirdan kucuk deger girin): " RESET);
            max = r_int();
            max = (max < 0) ? -1 : max;
            limit_houses(&house_p, kriter, min, max, &list_lenght);
            printf(GREEN "\nListe "BLUE "%d - %d" GREEN" araliginda limitlendi, bu aralikta" MAGENTA " %d " GREEN "ev bulunuyor\n" RESET, (min < 0) ? -1 : min, (max < 0) ? -1 : max, list_lenght);
            sub_menu = KRITER_EKLENSIN_MI;
          } else if (kriter == 2 || kriter == 4) { //String tipi kriterler
            printf(CYAN "\nAlt limit karakter nedir (A-Z) (atlamak icin A girin): " RESET);
            min_c = r_char();
            min_c = (min_c < 'A' || min_c > 'Z') ? -1 : min_c;
            printf(CYAN "\nUst limit karakter nedir (A-Z) (atlamak icin Z girin): " RESET);
            max_c = r_char();
            max_c = (max_c < 'A' || max_c > 'Z') ? -1 : max_c;
            limit_houses(&house_p, kriter, min_c, max_c, &list_lenght);
            printf(GREEN "\nListe "BLUE "%c - %c" GREEN" araliginda limitlendi, bu aralikta" MAGENTA " %d " GREEN "ev bulunuyor\n" RESET, min_c, max_c, list_lenght);
            sub_menu = KRITER_EKLENSIN_MI;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = KRITER_SEC;
          }
        } else if( sub_menu == SIRALAMA_YONU_SEC) {
          printf( (sayac > 1) ? CYAN "\nHangi yonde sirali bastirmak istiyorsunuz? " MAGENTA"(birden cok kriter oldugu icin id ye gore siralanir)\n" : CYAN "\nHangi yonde sirali bastirmak istiyorsunuz? " MAGENTA "(secilen kritere gore siralanir)\n");
          printf(BLUE "\n1 - Artan\n");
          printf(BLUE "2 - Azalan\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();
          if (sayac != 1) kriter = ID;
          if (mini_menu == 1) {
            sort_houses(&house_p, kriter, ASC);
            sub_menu = BASILACAK_SAYISI_BELIRLE;
          } else if ( mini_menu == 2) {
            sort_houses(&house_p, kriter, DESC);
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
          mini_menu = r_int();
          if (mini_menu == 1) {
            sub_menu = DOSYA_ADRESI_AL;
          } else if (mini_menu == 2) {
            sub_menu = EKRANA_BASTIR;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          }
        } else if (sub_menu == EKRANA_BASTIR) {
          print_house(house_p, MULTI, basilacak_sayisi);
          sub_menu = TEKRAR_YAP_SOR;
        } else if( sub_menu == DOSYA_ADRESI_AL) {
          printf(CYAN "\nBastirilacak dosyanin adresini nasil belirlemek istersiniz?\n");
          printf(BLUE "\n1 - On tanimli adresi kullan (%s)\n", (verinin_bulundugu_grup == OGRENME_VERISI) ? CSV_FILTERED_TRAIN_OUT_PREDEFINED : CSV_FILTERED_TEST_OUT_PREDEFINED);
          printf(BLUE "2 - Elle gir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();
          if (mini_menu == 1) {
            strcpy(csv_out_directory, (verinin_bulundugu_grup == OGRENME_VERISI) ? CSV_FILTERED_TRAIN_OUT_PREDEFINED : CSV_FILTERED_TEST_OUT_PREDEFINED);
            sub_menu = DOSYAYA_YAZDIR;
          } else if (mini_menu == 2) {
            printf(MAGENTA "\nBastırma adresi : " RESET);
            scanf("%s", csv_out_directory);
            if (check_csv(csv_out_directory)) {
              sub_menu = DOSYAYA_YAZDIR;
            } else {
              printf(RED "\nGirilen adres bir csv dosyasi degil\n" RESET);
              sub_menu = DOSYA_ADRESI_AL;
            }            
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = DOSYA_ADRESI_AL;
          }
        } else if (sub_menu == DOSYAYA_YAZDIR) {
          write_house_to_file(house_p, csv_out_directory, basilacak_sayisi);
          printf(GREEN "\n%s dosyasina ev bilgileri yazildi\n" RESET, csv_out_directory);
          sub_menu = TEKRAR_YAP_SOR;
        } else if( sub_menu == TEKRAR_YAP_SOR) {
          printf(CYAN "\nBaska bir listeleme daha yapmak ister misiniz? \n");
          printf(BLUE "\n1 - Evet\n");
          printf(BLUE "2 - Hayir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();

          if(mini_menu == 1) {
            sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC; //basa don
          } else if (mini_menu == 2) {
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
    } else if ( menu == ID_VERILEN_EVI_BUL){
      printf(GREEN "\n\nID DEGERI VERILEN EVI BASTIRMA\n");

      //fonksyonda kullanilacak degiskenler baslangic degerlerine donduruluyor
      sub_menu = ID_AL;
      
      while( sub_menu != EXIT) {
        if( sub_menu == ID_AL) {
          printf(CYAN "\nBastirmak istediginiz evin ID si: "RESET);
          id = r_int();
          sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC;
        } else if( sub_menu == KULLANILACAK_VERININ_GRUBUNU_SEC) {
          printf(CYAN "\nBastirmak istediginiz ev hangi veri grubunda? \n");
          printf(BLUE "\n1 - Ogrenme Verileri\n");
          printf(BLUE "2 - Test Verileri\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();

          if(mini_menu == 1) {
            print_house(get_house_byid(id, housesById), SINGLE_WITH_TOP, LIMITLESS);
            sub_menu = TEKRAR_YAP_SOR;
          } else if (mini_menu == 2) {
            print_house(get_house_byid(id, housesById_test), SINGLE_WITH_TOP, LIMITLESS);
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
          mini_menu = r_int();

          if(mini_menu == 1) {
            sub_menu = ID_AL; //basa don
          } else if (mini_menu == 2) {
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
          id = r_int();
          sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC;
        } else if( sub_menu == KULLANILACAK_VERININ_GRUBUNU_SEC) {
          printf(CYAN "\nBu ev hangi veri grubunda? " MAGENTA "(komsular her zaman ogrenme verilerinden alinir)\n");
          printf(BLUE "\n1 - Ogrenme Verileri\n");
          printf(BLUE "2 - Test Verileri\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();

          if(mini_menu == 1) {
            house_p = get_neighborhoods(get_house_byid(id, housesById), housesByNeighbor, &list_lenght);
            if (house_p == NULL) {
              printf(RED "\nGirilen ev veri grubunda bulunmuyor veya bir komsusu yok\n" RESET);
              sub_menu = TEKRAR_YAP_SOR;
            } else {
              sub_menu = BASILACAK_SAYISI_BELIRLE;
            }
          } else if (mini_menu == 2) {
            house_p = get_neighborhoods(get_house_byid(id, housesById_test), housesByNeighbor, &list_lenght);
            if (house_p == NULL) {
              printf(RED "\nGirilen ev veri grubunda bulunmuyor veya bir komsusu yok\n" RESET);
              sub_menu = TEKRAR_YAP_SOR;
            } else {
              sub_menu = BASILACAK_SAYISI_BELIRLE;
            }
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
          mini_menu = r_int();
          if (mini_menu == 1) {
            sub_menu = DOSYA_ADRESI_AL;
          } else if (mini_menu == 2) {
            print_house(house_p, MULTI, basilacak_sayisi);
            sub_menu = TEKRAR_YAP_SOR;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          }
        } else if( sub_menu == DOSYA_ADRESI_AL) {
          printf(CYAN "\nBastirilacak dosyanin adresini nasil belirlemek istersiniz?\n");
          printf(BLUE "\n1 - On tanimli adresi kullan (%s)\n", CSV_LIST_NEIGHBORS_OUT_PREDEFINED);
          printf(BLUE "2 - Elle gir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();
          if (mini_menu == 1) {
            strcpy(csv_out_directory, CSV_LIST_NEIGHBORS_OUT_PREDEFINED);
            sub_menu = DOSYAYA_YAZDIR;
          } else if (mini_menu == 2) {
            printf(MAGENTA "\nBastırma adresi : " RESET);
            scanf("%s", csv_out_directory);
            if (check_csv(csv_out_directory)) {
              sub_menu = DOSYAYA_YAZDIR;
            } else {
              printf(RED "\nGirilen adres bir csv dosyasi degil\n" RESET);
              sub_menu = DOSYA_ADRESI_AL;
            }            
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = DOSYA_ADRESI_AL;
          }
        } else if (sub_menu == DOSYAYA_YAZDIR) {
          write_house_to_file(house_p, csv_out_directory, basilacak_sayisi);
          printf(GREEN "\n%s dosyasina ev bilgileri yazildi\n" RESET, csv_out_directory);
          sub_menu = TEKRAR_YAP_SOR;
        } else if( sub_menu == TEKRAR_YAP_SOR) {
          printf(CYAN "\nBaska bir evin daha komsularini listelemek ister misiniz? \n");
          printf(BLUE "\n1 - Evet\n");
          printf(BLUE "2 - Hayir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();

          if(mini_menu == 1) {
            sub_menu = ID_AL; //basa don
          } else if (mini_menu == 2) {
            sub_menu = -1; //fonksyondan cik
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = 5; //tekrar sor
          }

        } else {
          printf(RED "\nHATA : Tanimsiz alt menu - Ana menuye donuluyor\n" RESET);
          sub_menu = EXIT;
          menu = ANA_MENU;
        }
      }
      menu = ANA_MENU;
    } else if( menu == GRUPLANDIRARAK_GOSTER){
      printf(GREEN "\n\nKRITERE GORE GRUPLANDIRARAK ORTALAMA FIYATLARI LISTELEME\n");

      //fonksyonda kullanilacak degiskenler baslangic degerlerine donduruluyor
      sub_menu = KRITER_SEC;
      while( sub_menu != EXIT) {
        if( sub_menu == KRITER_SEC) {
          printf(CYAN "\nHangi kritere gore gruplandirilsin\n");
          printf(BLUE "\n1 - LOTAREA \n2 - STREET \n4 - NEIGHBORHOOD\n5 - YEARBUILT\n6 - OVERALLQUAL\n7 - OVERALLCOND\n8 - KITCHENQUAL\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          kriter = r_int();
          if( kriter >= 1 || kriter == 5 || kriter == 6 || kriter == 7 || kriter == 8 || kriter == 2 || kriter == 4) {
            sub_menu = EKRANA_BASTIR;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = KRITER_SEC;
          }
        } else if( sub_menu == EKRANA_BASTIR) {
          printf(GREEN "\nGruplandririlmis fiyat ortalamalari bastiriliyor\n" RESET);
          if (kriter == NEIGHBORHOOD) {
            mean_sale_prices(housesByNeighbor, NEIGHBORHOOD);
          } else {
            mean_sale_prices(housesById, kriter);
          }
          sub_menu = TEKRAR_YAP_SOR;
        } else if( sub_menu == TEKRAR_YAP_SOR) {
          printf(CYAN "\n\nBaska bir listeleme daha yapmak ister misiniz? \n");
          printf(BLUE "\n1 - Evet\n");
          printf(BLUE "2 - Hayir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();

          if(mini_menu == 1) {
            sub_menu = KRITER_SEC; //basa don
          } else if (mini_menu == 2) {
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
      } menu = ANA_MENU;
    } else if( menu == FIYAT_TAHMINI_YAP){
      printf(GREEN "\n\nFIYAT TAHMINI YAPMA\n");

      //fonksyonda kullanilacak degiskenler baslangic degerlerine donduruluyor
      sub_menu = TAHMIN_YONTEMI_SEC;
      
      while( sub_menu != EXIT) {
        if( sub_menu == TAHMIN_YONTEMI_SEC) {
          printf(CYAN "\nHangi yontemle tahmin yapmak istersiniz? \n");
          printf(BLUE "\n1 - Evlerin benzerliklerine gore " MAGENTA "(id / test verileri)\n");
          printf(BLUE "2 - Dogrusal bir iliski kurarak " MAGENTA" (test verileri)\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();

          if(mini_menu == 1) {
            yontem = BENZERLIK_YONTEMI;
            sub_menu = KULLANILACAK_VERIYI_SEC;
          } else if (mini_menu == 2) {
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
          mini_menu = r_int();

          if(mini_menu == 1) {
            kullanilacak_veri = ID;
            sub_menu = ID_AL;
          } else if (mini_menu == 2) {
            kullanilacak_veri = TEST_VERISI;
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          } else {
            printf(RED "\nTanimsiz secim\n" RESET);
            sub_menu = KULLANILACAK_VERIYI_SEC;
          } 
        }else if( sub_menu == ID_AL) {
          printf(CYAN "\nFiyat tahmini yapmak istediginiz evin ID si: "RESET);
          id = r_int();
          sub_menu = KULLANILACAK_VERININ_GRUBUNU_SEC;
        } else if ( sub_menu == KULLANILACAK_VERININ_GRUBUNU_SEC) {
          printf(CYAN "\nBu ev hangi veri grubunda? \n");
          printf(BLUE "\n1 - Ogrenme Verileri\n");
          printf(BLUE "2 - Test Verileri\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();
          if (mini_menu == 1) {
            verinin_bulundugu_grup = OGRENME_VERISI;
            sub_menu = SONUC_GORUNTULEME_BICIMI_SEC;
          } else if (mini_menu == 2) {
            verinin_bulundugu_grup = TEST_VERISI;
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
          mini_menu = r_int();
          if (mini_menu == 1) {
            goruntuleme_bicimi = DOSYAYA_YAZDIR;
            sub_menu = DOSYA_ADRESI_AL;
          } else if (mini_menu == 2) {
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
          mini_menu = r_int();
          if (mini_menu == 1) {
            strcpy(csv_out_directory, (yontem == DOGRUSALLIK_YONTEMI) ? CSV_LINEER_OUT_PREDEFINED : CSV_RELATIONAL_OUT_PREDEFINED);
            sub_menu = yontem;
          } else if (mini_menu == 2) {
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
          house_p = linearise_hash_table(housesById, ID, &list_lenght);
          house_p_2 = linearise_hash_table(housesById_test, ID, &list_lenght);
          sort_houses(&house_p, ID, ASC);
          matrix_p = calculate_parameter(house_p);
          printf(BLUE "\n\nTahmin dogrusunun XY duzlemi uzerinde gosterimi:\n" MAGENTA"  Y = %.2lfX + %.2lf" RESET, matrix_p->values[1][0], matrix_p->values[0][0]);
          Matrix* matrix_p_2 = make_prediction(&house_p_2, matrix_p);
          matrix_to_house_list(matrix_p_2, &house_p_2);        
          write_house_to_file (house_p_2, csv_out_directory, basilacak_sayisi);
          printf(GREEN "\n%s dosyasina ev bilgileri yazildi\n" RESET, csv_out_directory);
          sub_menu = TEKRAR_YAP_SOR;
        } else if ( sub_menu == BENZERLIK_YONTEMI ) {
          if(kullanilacak_veri == ID) {
            house_p = get_house_byid(id, (verinin_bulundugu_grup == TEST) ? housesById_test : housesById);
            tahmin = model_by_similarity(housesByNeighbor,  house_p);
            if(goruntuleme_bicimi == EKRANA_BASTIR) {
              print_house (house_p, SINGLE_WITH_TOP, LIMITLESS);
              printf (CYAN "\nTahmin fiyati %d \n" RESET, tahmin);
              sub_menu = TEKRAR_YAP_SOR;
            } else if (goruntuleme_bicimi == DOSYAYA_YAZDIR) {
              write_house_to_file (house_p, csv_out_directory, basilacak_sayisi);
              printf(GREEN "\n%s dosyasina ev bilgileri yazildi\n" RESET, csv_out_directory);
              sub_menu = TEKRAR_YAP_SOR;
            } else {
              printf(RED "\nHATA : Sonuc goruntuleme bicimi eksigi hatasi - Ana menuye donuluyor\n" RESET);
              sub_menu = EXIT;
              menu = ANA_MENU;
            }
          } else if (kullanilacak_veri == TEST_VERISI) {
            house_p = linearise_hash_table(housesById_test, ID, &list_lenght);
            house_p_2 = house_p;
            for (int i = 0; i < list_lenght; i++) {
              house_p_2 = get_house_byid(id, (verinin_bulundugu_grup == TEST) ? housesById_test : housesById);
              house_p_2->saleprice = model_by_similarity(housesByNeighbor,  house_p);
              house_p_2 = house_p_2->nextHouse;
            }
            if(goruntuleme_bicimi == DOSYAYA_YAZDIR) {
              write_house_to_file(house_p, csv_out_directory, basilacak_sayisi);
              printf(GREEN "\n%s dosyasina ev bilgileri yazildi\n" RESET, csv_out_directory);
              sub_menu = TEKRAR_YAP_SOR;
            } else if(goruntuleme_bicimi == EKRANA_BASTIR) {
              print_house (house_p, MULTI, LIMITLESS);
              sub_menu = TEKRAR_YAP_SOR;
            } else {
              printf(RED "\nHATA : Sonuc goruntuleme bicimi eksigi hatasi - Ana menuye donuluyor\n" RESET);
              sub_menu = EXIT;
              menu = ANA_MENU;
            }
          }
        } else if( sub_menu == TEKRAR_YAP_SOR) {
          printf(CYAN "\nBaska bir fiyat tahmini daha yapmak ister misiniz? \n");
          printf(BLUE "\n1 - Evet\n");
          printf(BLUE "2 - Hayir\n");
          printf(MAGENTA "\nSeciminiz: " RESET);
          mini_menu = r_int();

          if(mini_menu == 1) {
            menu = FIYAT_TAHMINI_YAP; //basa don
            sub_menu = TAHMIN_YONTEMI_SEC;
          } else if (mini_menu == 2) {
            menu = ANA_MENU; //islevden cik
            sub_menu = EXIT;
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
    } else if( menu == GELISTIRICI_SECENEKLERI){
      int *ids, *prices;
      mean_sale_prices(housesById, NEIGHBORHOOD);
      menu = ANA_MENU;
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

//int okur. Kaışık bir veri girişinde sadece rakamları alır, rakam girilmezse tekrar scanf yapar
int r_int () {
  int read;
  if(!scanf("%d",&read)==1) scanf("%*[^0-9] %d",&read);
  return read;  
}

//char okur (sadece a-Z arasini)
char r_char () {
  char read;
  scanf("%*[^A-Z] %c",&read);
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