#include"dataset.h"
#include<stdio.h>
#include<stdlib.h>

#define HASH_TABLE_SIZE_TYPE_ID 100
#define HASH_TABLE_SIZE_TYPE_NEIGHBOR ('Z' - 'A') * 2
#define LINE_BUFFER_SIZE 1024


//Verilen id için hash değeri döndürür
int hash_code(int id) {
  return id % HASH_TABLE_SIZE_TYPE_ID;
}

//komşuluk değeri için hash değeri döndürür
int hash_code_n(char * n) {
  return ((n[0] - 'A') * 2) % HASH_TABLE_SIZE_TYPE_NEIGHBOR;
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
void place_house (House * house, House * houses[], int hash_type) { 
  House * tmp_hp;

  int hashIndex;

  switch (hash_type)
  {
  case HASH_TYPE_ID:
    hashIndex = hash_code(house->id);
    if(houses[hashIndex] == NULL) {
      houses[hashIndex] = house;
    } else {
      tmp_hp = houses[hashIndex];
      while (tmp_hp->nextHouseById != NULL)
      {
        tmp_hp = tmp_hp->nextHouseById;
      }
      tmp_hp->nextHouseById = house;
    }
    break;

  case HASH_TYPE_NEIGHBORHOODS:
    hashIndex = hash_code_n(house->neighborhood);
    int c = 0;
    while (houses[hashIndex] != NULL && strcmp(houses[hashIndex]->neighborhood, house->neighborhood) != 0) {
      hashIndex++;
      hashIndex %= HASH_TABLE_SIZE_TYPE_NEIGHBOR;
    }
    
    if(houses[hashIndex] == NULL) {
      houses[hashIndex] = house;
    } else {
      tmp_hp = houses[hashIndex];
      while (tmp_hp->nextHouseByNeighbor != NULL) {
        tmp_hp = tmp_hp->nextHouseByNeighbor;
      }
      tmp_hp->nextHouseByNeighbor = house;
    }
    break;
  default:
    hashIndex = house->id;
    break;
  }
}


//Csv dosyalarındaki evlerin verisini okuma fonksyonu
void read_house_data(char* filename, House * hById[], House * hByN[]){
  char buffer[LINE_BUFFER_SIZE];
  House * tmp;
  
  FILE *fp = fopen( filename, "r"); //dosyayı okumak için açıyoruz

  if (!(strcmp(filename, "../data/data_train.csv"))) { //train açılmış demektir

    fgets(buffer, LINE_BUFFER_SIZE, fp); //Veri olmayan ilk satırı okuyup atlıyoruz

    while(!feof(fp)){ //dosyanın sonuna kadar okuma yapar
      fgets(buffer, LINE_BUFFER_SIZE, fp); 
      tmp = write_house(buffer);
      place_house(tmp, hById, HASH_TYPE_ID);
      place_house(tmp, hByN, HASH_TYPE_NEIGHBORHOODS);
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

//verilen hash table ı House yapılarındaki NextHouse pointerını değiştirerek link list hale getirir
House* linearise_hash_table (House * ht[], int hash_type) {
  House * tmp;
  int counter;
  switch (hash_type)
  {
  case HASH_TYPE_ID:
    
    counter = ht[0]->id;
    tmp = get_house_byid(counter, ht);
    while (get_house_byid(counter, ht) != NULL) {    
      tmp->nextHouse = get_house_byid(counter, ht);
      counter++;
      tmp = tmp->nextHouse;
    }
    tmp->nextHouse = NULL;
    return ht[0];
    break;
  
  default:
    break;
  }
}

//Aldığı hash table ın haritasını çıkardır (elemanların id lerini bastırır)
void create_hash_table_tree(House * houses[], int hash_type) {


  /*  berkay-yildiz:
  burayı yapıyı kontrol etmek için yazdım.
  Daha sonrasında hash table ı başka şekillerde hashlarsak buradan nasıl bir yapı oluştuğunu kolayca görebiliriz.
  */
  int size = (hash_type == HASH_TYPE_ID) ? HASH_TABLE_SIZE_TYPE_ID : HASH_TABLE_SIZE_TYPE_NEIGHBOR;
  for(int i = 0; i < size; i++) {
    House * tmp;
    printf("\nhash: %-3d| ", i);
    if(houses[i] == NULL) printf ("null");
    else {
      tmp = houses[i];
      printf("%-5d", tmp->id);
      if(hash_type == HASH_TYPE_ID) {
        while(tmp->nextHouseById != NULL) {
          tmp = tmp->nextHouseById;
          printf("%-5d", tmp->id);
        }
      } else if(hash_type == HASH_TYPE_NEIGHBORHOODS) {
        while(tmp->nextHouseByNeighbor != NULL) {
          tmp = tmp->nextHouseByNeighbor;
          printf("%-7s", tmp->neighborhood);
        }
      }
      
    }
  }
}


//Aldığı evi ekrana yazdıran fonksyon
void print_house(House * house, int style, int limit){

  if(style) {
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
  }
  
  if(style == MULTI) {
    if (limit == LIMITLESS) {
      while (house->nextHouse != NULL) {
        printf(
          "%-15d%-15d%-15s%-15d%-15s%-15d%-15d%-15d%-15s\n", //Alınan evin verisini yazdırıyoruz
          house->id,
          house->lotarea,
          house->street,
          house->saleprice,
          house->neighborhood,
          house->yearbuilt,
          house->overallqual,
          house->overallcond,
          convert_kitchenqual_back(house->kitchenqual)
        );
        house = house->nextHouse;
      }
      print_house(house, SINGLE_WITHOUT_TOP, LIMITLESS);//döngüde basılmayan son değeri baskıyoruz
    } else {
      for (int i = 0; i < limit; i++) {
        printf(
          "%-15d%-15d%-15s%-15d%-15s%-15d%-15d%-15d%-15s\n", //Alınan evin verisini yazdırıyoruz
          house->id,
          house->lotarea,
          house->street,
          house->saleprice,
          house->neighborhood,
          house->yearbuilt,
          house->overallqual,
          house->overallcond,
          convert_kitchenqual_back(house->kitchenqual)
        );
        house = house->nextHouse;
      }
    }
    
  }else if(style == SINGLE_WITH_TOP || style == SINGLE_WITHOUT_TOP){
    printf(
      "%-15d%-15d%-15s%-15d%-15s%-15d%-15d%-15d%-15s\n", //Alınan evin verisini yazdırıyoruz
      house->id,
      house->lotarea,
      house->street,
      house->saleprice,
      house->neighborhood,
      house->yearbuilt,
      house->overallqual,
      house->overallcond,
      convert_kitchenqual_back(house->kitchenqual)
    );
  }
  
  
}

//Id si verilen evi döndüren fonksyon
House* get_house_byid(int id, House * houses[]){
  int hashIndex = hash_code(id);
  House * tmp_house; 
  
  if(houses[hashIndex] != NULL) { //verilen hash değeri için tablodaki pointer NULL mu bakıyoruz
    tmp_house = houses[hashIndex]; //NULL değilse adresi geçici bir pointera alıyoruz
    if (tmp_house->id == id) return tmp_house; //elimizdeki adresteki değer istediğimiz id ile eşleşiyor mu bakıyoruz eşleşiyorsa döndürüyoruz
    while (tmp_house->nextHouseById != NULL) { //eşleşmiyorsa sonraki adres varsa oraya bakıyoruz
      tmp_house = tmp_house->nextHouseById;
      if (tmp_house->id == id) return tmp_house;
    }
  }

  /* berkay-yildiz:
    eğer tabloda olmayan bir id girilirse pointerda kalan adres geri dönüyor, çok önemli sayılmaz ama sona doğru halledilmeli
   */

}

House* get_neighborhoods(House * house, House * houses[]){
  int hashIndex = hash_code_n(house->neighborhood);
  while (strcmp(houses[hashIndex]->neighborhood, house->neighborhood) != 0) {
    hashIndex++;
    hashIndex %= HASH_TABLE_SIZE_TYPE_NEIGHBOR;
  }
  House * tmp = houses[hashIndex];
  while (tmp->nextHouseByNeighbor != NULL) {
    tmp->nextHouse = tmp->nextHouseByNeighbor;
    tmp = tmp->nextHouseByNeighbor;
  }
  tmp->nextHouse = NULL;

  return houses[hashIndex];
}

void mean_sale_prices(House* houses_head, int criter_name){
  int sum = 0;
  int counter = 0;
  int counter_general = 0;
  int tmp_i;
  char tmp_c[15];

  FILE * fp = fopen("mean_sale_prices_results.txt", "w");
  sort_houses(&houses_head, criter_name);

  while (houses_head->nextHouse != NULL) {
    switch (criter_name)
    {
    case LOTAREA:
      if(houses_head->lotarea != tmp_i) {
        if(counter_general != 0) {
          printf("%d | %d\n", tmp_i, sum/counter);
          fprintf(fp, "%d | %d\n", tmp_i, sum/counter);
        } 
        tmp_i = houses_head->lotarea;
        sum = 0;
        counter = 0;
      }
      sum += houses_head->saleprice;
      counter++;
      break;
    case STREET:
      if(!strcmp(houses_head->street,tmp_c)) {
        if(counter_general != 0) {
          printf("%s | %d\n", tmp_c, sum/counter);
          fprintf(fp, "%s | %d\n", tmp_c, sum/counter);
        } 
        strcpy(tmp_c,houses_head->street);
        sum = 0;
        counter = 0;
      }
      sum += houses_head->saleprice;
      counter++;
      break;
    case NEIGHBORHOOD:

      break;
    case YEARBUILT:

      break;
    case OVERALLQUAL:

      break;
    case OVERALLCOND:

      break;
    case KITCHENQUAL:

      break;
    
    default:
      break;
    }
    counter++;
    houses_head = houses_head->nextHouse;
  }
}


void sort_houses(House** houses, int criter_name){

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