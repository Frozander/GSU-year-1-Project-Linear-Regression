#include"dataset.h"
#include<stdio.h>
#include<stdlib.h>



//Verilen id için hash değeri döndürür
int hash_code(int id) {
  return id % HASH_TABLE_SIZE;
}

//komşuluk değeri için hash değeri döndürür
int hash_code_n(char * n) {
  return ((n[0] - 'A') * 2) % HASH_TABLE_SIZE_TYPE_NEIGHBOR;
}

//Aldığı csv dosyası satırındaki evi çözümleyip, ev için oluşturduğu alana bilgileri yazan ve alanın pointerını döndüren fonksyon
House * write_house(char * c, int file_type) {
  House * tmp_house = (House*) malloc(sizeof(House));

  const char s[2] = ",";
  char *token;

  token = strtok(c, s);

  int i = 0;
  while( token != NULL ) {

      if (i==0) tmp_house->id = atoi(token);
      if (i==1) tmp_house->lotarea = atoi(token);
      if (i==2) strncpy(tmp_house->street, token, sizeof(tmp_house->street));
      if (i==3) {
        if (file_type == TRAIN) tmp_house->saleprice = atoi(token);
        else if (file_type == TEST) i = 4;
      }
      
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
  case ID:
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

  case NEIGHBORHOOD:
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
void read_house_data(char* filename, House * hById[], House * hByN[], int file_type){
  char buffer[LINE_BUFFER_SIZE];
  House * tmp;
  
  FILE *fp = fopen( filename, "r"); //dosyayı okumak için açıyoruz

  if (file_type == TRAIN || file_type == TEST) { //train açılmış demektir

    fgets(buffer, LINE_BUFFER_SIZE, fp); //Veri olmayan ilk satırı okuyup atlıyoruz

    while(!feof(fp)){ //dosyanın sonuna kadar okuma yapar
      fgets(buffer, LINE_BUFFER_SIZE, fp); 
      if(buffer[0] != '\0'){
        tmp = write_house(buffer, file_type);
        place_house(tmp, hById, ID);
        place_house(tmp, hByN, NEIGHBORHOOD);
      }
      strcpy(buffer, "");
    }

    /* berkay-yildiz:
      Burada fscanf yerine fgets tercih etme sebebim fscanf in dosya sonu konusunda sıkıntı çıkarabilmesiydi,
        fgets direk satırı okuduğu için daha iyi bir yol.
      Keza genel olarak formatlı okuma stringleri pointerla eriştiğimiz alana yazmakta sorun çıkartıyordu,
        bunu strcpy ile yapmak daha sağlıklı olacağından fgets ile aldığım veriyi başka bir fonksyonda
        string ayıklama yaparak işledim. Bu şekilde daha çok kontrol olanağı oluyor ve hatalardan kaçınıyoruz.
     */

  } else {
    printf ("dosya tipi hatasi");
  }
  
  fclose(fp);
}


//verilen hash table ı House yapılarındaki NextHouse pointerını değiştirerek link list hale getirir
House* linearise_hash_table (House * ht[], int hash_type, int * lenght) {

  House * tmp;
  int counter = 0;
  for(int i = 0; i < HASH_TABLE_SIZE; i++) {
    tmp = ht[i];
    while(ghc_p(tmp, hash_type) != NULL ) {
      tmp->nextHouse = ghc_p(tmp, hash_type);
      tmp = ghc_p(tmp, hash_type);
      counter ++;
    }
    tmp->nextHouse = ht[i+1];
    counter ++;
  }
  tmp->nextHouse = NULL;
  *lenght = counter;
  return ht[0];
}


//Aldığı hash table ın haritasını çıkardır (elemanların id lerini bastırır)
void create_hash_table_tree(House * houses[], int hash_type) {


  /*  berkay-yildiz:
  burayı yapıyı kontrol etmek için yazdım.
  Daha sonrasında hash table ı başka şekillerde hashlarsak buradan nasıl bir yapı oluştuğunu kolayca görebiliriz.
  */
  int size = (hash_type == ID) ? HASH_TABLE_SIZE : HASH_TABLE_SIZE_TYPE_NEIGHBOR;
  for(int i = 0; i < size; i++) {
    House * tmp;
    printf("\nhash: %-3d| ", i);
    if(houses[i] == NULL) printf ("null");
    else {
      tmp = houses[i];
      printf("%-5d", tmp->id);
      if(hash_type == ID) {
        while(tmp->nextHouseById != NULL) {
          tmp = tmp->nextHouseById;
          printf("%-5d", tmp->id);
        }
      } else if(hash_type == NEIGHBORHOOD) {
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

  if(house != NULL) {
    if(style) {
      printf( CYAN
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

    int color = 0;
    
    if(style == MULTI) {
      if (limit == LIMITLESS) {
        while (house != NULL) {
          printf( (color%2 == 0) 
            ? GREEN "%-15d%-15d%-15s%-15d%-15s%-15d%-15d%-15d%-15s\n"
            : BLUE "%-15d%-15d%-15s%-15d%-15s%-15d%-15d%-15d%-15s\n", //Alınan evin verisini yazdırıyoruz
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
          color++;
        }
      } else {
        for (int i = 0; house != NULL && i < limit; i++) {
          printf((color%2 == 0) 
            ? GREEN "%-15d%-15d%-15s%-15d%-15s%-15d%-15d%-15d%-15s\n"
            : BLUE "%-15d%-15d%-15s%-15d%-15s%-15d%-15d%-15d%-15s\n", //Alınan evin verisini yazdırıyoruz
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
          color++;
        }
      }
      
    }else if(style == SINGLE_WITH_TOP || style == SINGLE_WITHOUT_TOP){
      printf( GREEN
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
  }else {
    if(style == SINGLE_WITH_TOP || style == SINGLE_WITHOUT_TOP) {
      printf(RED "\nBastirilmak istenilen ev yok\n" RESET);
    } else {
      printf(RED "\nBastirilmak istenilen ev dizisi bos\n" RESET);
    }
  }  
}

void write_house_to_file(House* head, char* filename, int limit){
  FILE* stream = fopen(filename, "w");

  fprintf(stream, "id,lotarea,street,saleprice,neighborhood,yearbuilt,overallqual,overallcond,kitchenqual\n");
  if (limit == LIMITLESS) {
    while (head != NULL)  {
      fprintf(stream,
              "%d,%d,%s,%d,%s,%d,%d,%d,%s\n", //Alınan evin verisini yazdırıyoruz
              head->id,
              head->lotarea,
              head->street,
              head->saleprice,
              head->neighborhood,
              head->yearbuilt,
              head->overallqual,
              head->overallcond,
              convert_kitchenqual_back(head->kitchenqual)
            );
      head = head->nextHouse;
    }
  } else {
    for (int i = 0; head != NULL && i < limit; i++)  {
      fprintf(stream,
              "%d,%d,%s,%d,%s,%d,%d,%d,%s\n", //Alınan evin verisini yazdırıyoruz
              head->id,
              head->lotarea,
              head->street,
              head->saleprice,
              head->neighborhood,
              head->yearbuilt,
              head->overallqual,
              head->overallcond,
              convert_kitchenqual_back(head->kitchenqual)
            );
      head = head->nextHouse;
    }
  }

  fclose(stream);
  
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
    return NULL;
  } else {
    return NULL;
  }
}

//Verilen evin komşularını bağlı liste olarak döndüren fonksyon
House* get_neighborhoods(House * house, House * houses[], int * lenght){
  if(house != NULL) {
    int hashIndex = hash_code_n(house->neighborhood);
    int counter = 1;
    while (strcmp(houses[hashIndex]->neighborhood, house->neighborhood) != 0) {
      hashIndex++;
      hashIndex %= HASH_TABLE_SIZE_TYPE_NEIGHBOR;
    }
    House * tmp = houses[hashIndex];
    while (tmp->nextHouseByNeighbor != NULL) {
      tmp->nextHouse = tmp->nextHouseByNeighbor;
      tmp = tmp->nextHouseByNeighbor;
      counter ++;
    }
    tmp->nextHouse = NULL;
    *lenght = counter;
    return houses[hashIndex];
  } else {
    return NULL;
  }
  
}


//İstenilen alt üst değere göre verilen bağlı listeyi kırpar, NON kullanılarak min veya max belirlenmeyebilir
void limit_houses(House** houses_head, int criter_name, int min, int max, int * new_lenght){
  int counter = 0;
  if(*houses_head != NULL) {
    House * head = *houses_head;
    House * keep;
    while (head->nextHouse != NULL) {
      if((min == NON || ghc_i(head, criter_name) >= min) && (max == NON || ghc_i(head, criter_name) <= max)) {
        
        if(counter == 0) {
          *houses_head = head;
          keep = head;
        } else {
          keep->nextHouse = head;
          keep = head;
        }
        counter++;
      }
      head = head->nextHouse;
    }
    if (counter > 0) {
      keep->nextHouse = NULL;
    } else {
      *houses_head = NULL;
    }
    *new_lenght = counter;
  }
}

int get_criter_avg(House* head, int criter) {

  int sum = 0;
  int counter = 0;
  while (head->nextHouse != NULL) {
    sum += ghc_i(head, criter);
    head = head->nextHouse;
    counter++;
  }
  if(counter != 0) {
    return sum/counter;
  } else {
    printf("crtier hatasi");
    return 0;
  }
  
}

int get_list_lenght (House * head) {
  if (head != NULL) {
    int counter = 1;
    while(head->nextHouse != NULL) {
      counter++;
      head = head->nextHouse;
    }
    return counter;
  }else {
    printf("head = NULL");
    return 0;
  }
  
}

int ghc_i_avg (House * house, int * criter_names) {
  int sum = 0;
  int counter = 0;
  while (counter < sizeof(criter_names) ) {
    sum += ghc_i(house, criter_names[counter]);  
    counter++;
  }

  return sum/counter;
}

//integer değerinde ev verisi döndürür
int ghc_i (House * house, int criter_name) {
  switch (criter_name)
    {
    case ID:
      return house->id;
      break;
    case LOTAREA:
      return house->lotarea;
      break;
    case SALEPRICE:
      return house->saleprice;
      break;
    case YEARBUILT:
      return house->yearbuilt;
      break;
    case OVERALLQUAL:
      return house->overallqual;
      break;
    case OVERALLCOND: 
      return house->overallcond;
      break;
    case KITCHENQUAL:
      return house->kitchenqual;
      break;
    case STREET:
      return *(house->street);
      break;
    case NEIGHBORHOOD:
      return *(house->neighborhood);
      break;
    default:
      break;
    }
}

House * ghc_p (House * house, int criter_name) {
  switch (criter_name)
  {
  case ID:
    return house->nextHouseById;
    break;
  case NEXT_HOUSE_BY_ID:
    return house->nextHouseById;
    break;
  case NEIGHBORHOOD:
    return house->nextHouseByNeighbor;
    break;
  case NEXT_HOUSE_BY_NEIGHBORHOOD:
    return house->nextHouseByNeighbor;
    break;
  case NEXT_HOUSE:
    return house->nextHouse;
    break;
  default:
    break;
  }
}

//string cinsinde ev verisi döndürür
char * ghc_s (House * house, int criter_name) {
  switch (criter_name)
    {
    case STREET:
      return house->street;
      break;
    case NEIGHBORHOOD:
      return house->neighborhood;
      break;
    default:
      break;
    }
}

void mean_sale_prices (House * ht[], int criter) {

  if( criter == NEIGHBORHOOD) {
    House * tmp;
    int sum, count, color;
    sum = color = count;
    printf(CYAN"\n\n%-20s%-15s%-15s", "Kriter Degeri", "Ev Sayisi", "Ortalama Fiyat");
    for(int i = 0; i < HASH_TABLE_SIZE_TYPE_NEIGHBOR; i++) {
      
      if( ht[i] != NULL) {
        tmp = ht[i];
        while(tmp != NULL) {
          sum += ghc_i(tmp, SALEPRICE);
          count++;
          tmp = tmp->nextHouseByNeighbor;
        }
        if (color%2 == 0) {
          printf(GREEN "\n%-20s%-15d%-15d", ht[i]->neighborhood, count, sum/count);
        } else {
          printf(BLUE "\n%-20s%-15d%-15d", ht[i]->neighborhood, count, sum/count);
        }
        color++;
      }
      
    }
  } else {
    House * tmp;
    int sum, count, l, color;
    sum = count = l = color = 0;
    tmp = linearise_hash_table(ht, ID, &l);
    sort_houses(&tmp, criter, ASC);
    printf(CYAN"\n\n%-20s%-15s%-15s", "Kriter Degeri", "Ev Sayisi", "Ortalama Fiyat");
    for (int i = 0; i < l-1; i++) {
      sum += tmp->saleprice;
      count++;
      if ( (criter == NEIGHBORHOOD || criter == STREET) ? strcmp(ghc_s(tmp, criter), ghc_s(tmp->nextHouse, criter)) : ghc_i(tmp, criter) != ghc_i(tmp->nextHouse, criter)) {
        if (criter == NEIGHBORHOOD || criter == STREET) {
          if (color%2 == 0) {
            printf(GREEN "\n%-20s%-15d%-15d", ghc_s(tmp, criter), count, sum/count);
          } else {
            printf(BLUE "\n%-20s%-15d%-15d", ghc_s(tmp, criter), count, sum/count);
          }
          color++;
        }else {
          if (color%2 == 0) {
            printf(GREEN "\n%-20d%-15d%-15d", ghc_i(tmp, criter), count, sum/count);
          } else {
            printf(BLUE "\n%-20d%-15d%-15d", ghc_i(tmp, criter), count, sum/count);
          }
          color++;
        }        
        sum = 0;
        count = 0;
      }
      tmp = tmp->nextHouse;
    }
    if (count > 0 && (criter == NEIGHBORHOOD || criter == STREET)) {
      if (color%2 == 0) {
        printf(GREEN "\n%-20s%-15d%-15d", ghc_s(tmp, criter), count, sum/count);
      } else {
        printf(BLUE "\n%-20s%-15d%-15d", ghc_s(tmp, criter), count, sum/count);
      }
      color++;
    }else if (count > 0) {
      if (color%2 == 0) {
        printf(GREEN "\n%-20d%-15d%-15d", ghc_i(tmp, criter), count, sum/count);
      } else {
        printf(BLUE "\n%-20d%-15d%-15d", ghc_i(tmp, criter), count, sum/count);
      }
      color++;
    }
  }
}

//link list olarak alınan evleri sıralayan fonksyon
void sort_houses(House** houses, int criter_name, int order){
  merge_sort(houses, criter_name, order);
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

//EV SIRALAMA FONKSYONLARI (MERGE SORT)
House* merge(House* in1, House* in2, int criter_name, int order){
  House* res = NULL;

  if (in1 == NULL) 
      return (in2); 
  else if (in2 == NULL) 
      return (in1);

  if (order == ASC)
    {
      if (ghc_i(in1, criter_name) <= ghc_i(in2, criter_name))
        {
          res = in1;
          res->nextHouse = merge(in1->nextHouse, in2, criter_name, order);
        } else
        {
          res = in2;
          res->nextHouse = merge(in1, in2->nextHouse, criter_name, order);
        }
        return (res);
    } else if (order == DESC)
    {
      if (ghc_i(in1, criter_name) >= ghc_i(in2, criter_name))
        {
          res = in1;
          res->nextHouse = merge(in1->nextHouse, in2, criter_name, order);
        } else
        {
          res = in2;
          res->nextHouse = merge(in1, in2->nextHouse, criter_name, order);
        }
    return (res);
    }
}

void merge_sort(House ** list_head_ref, int criter_name, int order){

  House* head = *list_head_ref;
  House* first_node;
  House* second_node;

  if ((head == NULL) || (head->nextHouse == NULL)) //Eğer liste boşsa veya bir elemanı varsa foksiyondan çıkar
  {
    return;
  }

  if(criter_name < ID || criter_name > KITCHENQUAL) criter_name = ID;

  //Listeyi ortadan ikiye bölüp first_node ve second_node pointerlarına atar
  split_list(head, &first_node, &second_node);
  
  merge_sort(&first_node, criter_name, order); //Her iki node için de recursive merge_sort çağırılır.
  merge_sort(&second_node, criter_name, order);

  *list_head_ref = merge(first_node, second_node, criter_name, order); //Recursion bittikten sonra listeler birleştirilir

}

void split_list(House* input, House** first_half, House** second_half){

  House* fast_node;
  House* slow_node;

  slow_node = input;
  fast_node = input->nextHouse;

  //Hızlı node iki ileri gider, yavaş node bir ileri gider
  //Sonuç olarak hızlı node son değeri gösterirken yavaş node ortanca - 1 değeri gösterir
  while (fast_node != NULL)
  {
    fast_node = fast_node->nextHouse;
    if (fast_node != NULL)
    {
      slow_node = slow_node->nextHouse;
      fast_node = fast_node->nextHouse;
    }
  }

  *first_half = input;
  *second_half = slow_node->nextHouse; // slow_node ortanca değerden bi geride olduğu için slow_node->nextHouse'u kullanıyoruz
  slow_node->nextHouse = NULL;
}