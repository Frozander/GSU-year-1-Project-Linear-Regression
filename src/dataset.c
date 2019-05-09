#include"dataset.h"
#include<stdio.h>
#include<stdlib.h>

#define HASH_TABLE_SIZE 100
#define LINE_BUFFER_SIZE 1024

//Verilen id için hash değeri döndürür
int hash_code(int id) {
  return id % HASH_TABLE_SIZE;
}

//Aldığı csv dosyası satırındaki evi çözümleyip, ev için oluşturduğu alana bilgileri yazan ve alanın pointerını döndüren fonksyon
House * write_house(char * c) {
  House * tmp_house = (House*) malloc(sizeof(House));

  const char s[2] = ",";
  char *token;

  token = strtok(c, s);

  int i = 0;
  while( token != NULL ) {

      if (i==0) tmp_house->id = atoi(token);
      if (i==1) tmp_house->lotarea = atoi(token);
      if (i==2) strncpy(tmp_house->street, token, sizeof(tmp_house->street));
      if (i==3) tmp_house->saleprice = atoi(token);
      if (i==4) strncpy(tmp_house->neighborhood, token, sizeof(tmp_house->neighborhood));
      if (i==5) tmp_house->yearbuilt = atoi(token);
      if (i==6) tmp_house->overallqual = atoi(token);
      if (i==7) tmp_house->overallcond = atoi(token);
      if (i==8) tmp_house->kitchenqual = convert_kitchenqual(token);
    
      token = strtok(NULL, s);
      i++;
  }

  return tmp_house;
}

//Aldığı ev pointerını hash tableda uygun yere yerleştiren fonksyon
void place_house (House * house, House * houses[] ) { 
  House * tmp_hp;

  int hashIndex = hash_code(house->id); //hashı alıyoruz


  if(houses[hashIndex] == NULL) {
    houses[hashIndex] = house;
  } else {
    tmp_hp = houses[hashIndex];
    while (tmp_hp->nextHouse != NULL)
    {
      tmp_hp = tmp_hp->nextHouse;
    }
    tmp_hp->nextHouse = house;
  }

}



//Csv dosyalarındaki evlerin verisini okuma fonksyonu
void read_house_data(char* filename, House* houses[]){
  char buffer[LINE_BUFFER_SIZE];
  
  FILE *fp = fopen( filename, "r"); //dosyayı okumak için açıyoruz

  if (!(strcmp(filename, "../data/data_train.csv"))) { //train açılmış demektir

    fgets(buffer, LINE_BUFFER_SIZE, fp); //Veri olmayan ilk satırı okuyup atlıyoruz

    while(!feof(fp)){ //dosyanın sonuna kadar okuma yapar
      fgets(buffer, LINE_BUFFER_SIZE, fp);
      place_house(write_house(buffer), houses); //okunan satırı alana yazma fonksyonu ile yazıp dönen House pointerını da yerleşirme fonksyonu ile hash_table içine yerleştiriyoruz
    }

    /* berkay-yildiz:
      Burada fscanf yerine fgets tercih etme sebebim fscanf in dosya sonu konusunda sıkıntı çıkarabilmesiydi,
        fgets direk satırı okuduğu için daha iyi bir yol.
      Keza genel olarak formatlı okuma stringleri pointerla eriştiğimiz alana yazmakta sorun çıkartıyordu,
        bunu strcpy ile yapmak daha sağlıklı olacağından fgets ile aldığım veriyi başka bir fonksyonda
        string ayıklama yaparak işledim. Bu şekilde daha çok kontrol olanağı oluyor ve hatalardan kaçınıyoruz.
     */

  } else if (!(strcmp(filename, "../data/data_test.csv"))) { //test açılmış demektir

  }
  
  fclose(fp);
}

//Aldığı hash table ın haritasını çıkardır (elemanların id lerini bastırır)
void create_hash_table_tree(House * houses[] ) {


  /*  berkay-yildiz:
  burayı yapıyı kontrol etmek için yazdım.
  Daha sonrasında hash table ı başka şekillerde hashlarsak buradan nasıl bir yapı oluştuğunu kolayca görebiliriz.
  */
  
  for(int i = 0; i < HASH_TABLE_SIZE; i++) {
    House * tmp;
    printf("\nhash: %-3d| ", i);
    if(houses[i] == NULL) printf ("null");
    else {
      tmp = houses[i];
      printf("%-5d", tmp->id);
      while(tmp->nextHouse != NULL) {
        tmp = tmp->nextHouse;
        printf("%-5d", tmp->id);
      }
    }
  }
}


//Aldığı evi ekrana yazdıran fonksyon
void print_house(House house){

  printf(
    "\n%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n", //Üst bilgi kısmını yazdırıyoruz
    "id",
    "lotarea",
    "street",
    "saleprice",
    "neighborhood",
    "yearbuilt",
    "overallqual",
    "overallcond",
    "kitchenqual"
  );

  printf(
    "%-15d%-15d%-15s%-15d%-15s%-15d%-15d%-15d%-15s\n", //Alınan evin verisini yazdırıyoruz
    house.id,
    house.lotarea,
    house.street,
    house.saleprice,
    house.neighborhood,
    house.yearbuilt,
    house.overallqual,
    house.overallcond,
    convert_kitchenqual_back(house.kitchenqual)
  );
  
}

//Id si verilen evi döndüren fonksyon
House get_house_byid(int id, House * houses[]){
  int hashIndex = hash_code(id);
  House * tmp_house; 
  
  if(houses[hashIndex] != NULL) {
    tmp_house = houses[hashIndex];
    if(tmp_house->nextHouse == NULL) {
      return *tmp_house;
    } else {
      while (tmp_house->nextHouse != NULL) {
        tmp_house = tmp_house->nextHouse;
        if (tmp_house->id == id) return *tmp_house;
      }
    }
  }
}

House* get_neighborhoods(House house){
  printf("Get neighborhoods of house with id %d\n",house.id);
  House * tmp;
  // TODO
  return tmp;
}

float* mean_sale_prices(House* houses,char* criter_name){
  printf("Calculate mean sale prices by %s \n",criter_name);
  float * tmp;
  // TODO
  return tmp;
}
void sort_houses(House* houses,char* criter_name){
  printf("Sort house by %s and save \n",criter_name);
  // TODO
}


//ikili karakter cinsinden alınan kitchenqual verisini 5-1 arası sayıya dönüştürür
int convert_kitchenqual (char * c) {
  switch (c[0])
  {
  case 'E':
    return 5;
    break;
  
  case 'G':
    return 4;
    break;

  case 'T':
    return 3;
    break;

  case 'F':
    return 2;
    break;

  case 'P':
    return 1;
    break;
  
  default:
    return 0;
    break;
  }
}

//5-1 arası sayı cinsinden alınan kitchenqual verisini ikili karakter cinsinden veriye dönüştürür
char * convert_kitchenqual_back (int value) {
  switch (value)
  {
  case 5:
    return "Ex";
    break;
  
  case 4:
    return "Gd";
    break;

  case 3:
    return "TA";
    break;

  case 2:
    return "Fa";
    break;

  case 1:
    return "Po";
    break;
  
  default:
    return 0;
    break;
  }
}