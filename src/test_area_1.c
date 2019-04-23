/*
berkay-yildiz:
dataset.c deki read_house_data fonksyonunun testi

derlemeye dahil edilecekler sırasıyla:
main.c
test_area_1.c

*/

#include"dataset.h"
#include<stdio.h>
#include<stdlib.h>

#define MAX_ATTRIBUTES 16
#define MAX_LINE 128

//csv dosyalarındaki evlerin verisini okuma fonksyonu
void read_house_data(char* filename, House houses[]){
  printf("Reading file %s\n",filename); //Okunan dosya adı bilgisi bastırılıyor

  FILE *openFile = fopen(filename, "r"); //Dosya açılıyor

  if (openFile == NULL) { //Dosyanın okuma düzeninde açılamaması durumu için hata ve fonksyondan çıkış
    printf("I/O Error: Could not read the target file '%s'\n", filename);
    return;
  }

  char readLine[MAX_LINE]; //Dosyadan ilk okunacak satırın aktarılacağı değişken
  int csv_attributes[MAX_ATTRIBUTES]; //Okunan satır ayıklandıktan sonra niteliklerin (attributes) aktarılacağı dizi.

  fgets(readLine, sizeof(readLine), openFile); //Dosyadan ilk satırı okuyoruz

	char delim[] = ","; //Satırdaki elemanları ayrıştırmak için kullanılan harfi belirliyoruz

	char *ptr = strtok(readLine, delim); //burada strtok ile ilk niteliği ptr içine kaydediyoruz

	for(int i = 0; ptr != NULL; i++) //ptr NULL olana (bitene) kadar devam ediyoruz, index niteliklerin olduğu diziyi ilerletmek için
	{
		printf("attribute name: %s\n", ptr); //Sıradaki nitelik adı basılıyor
    csv_attributes[i] = atoi(ptr); //Nitelik adı int e dönüştürülüp nitelikler dizisine kaydediliyor
    printf("attribute no:%d \n\n", csv_attributes[i]); //Niteliğin int formu basılıyor
		ptr = strtok(NULL, delim); //burada ptr içine bir sonraki niteliği geçiriyoruz

  /*
  berkay-yildiz:
  Burada nitelikleri sayıya çevirirme işlemini gerçekleştiremedim, build edip mainden test-no-1 i çağırırsan çıktıyı görebilirsin
  bunları sayıya çevridikten sonra buradaki eksiklikleri kontrol edebiliriz ve bu diziyi baz alarak formatlı okuma yapabiliriz.
  */

	}

	printf("\n\n");
  
  return; 
}




