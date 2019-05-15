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

void limit_list(House* houses_head, int criter_name, int min, int max){
  sort_houses(&houses_head, criter_name);

}

void mean_sale_prices(House* houses_head, int criter_name, int criter_data){
  //Lotarea ve Yearbuilt için
  int price_sum = 0;
  int avg_sum = 0;
  int keeper = 0;

  //Diğerleri için
  int counter = 0;
  int tmp_i = 0;
  int tmp_i_old = 0;
  char tmp_c[15];
  char tmp_c_old[15];

  FILE * fp = fopen("mean_sale_prices_results.txt", "w");
  sort_houses(&houses_head, criter_name, ASC);

  switch (criter_name)
    {
    case LOTAREA:
      keeper = houses_head->lotarea;
      printf("\033[1;31m"); //Kırmızı bastırmak için
      printf("\n%-25s%-25s%-25s%-25s%-25s\n", "Min Lotarea", "Max Lotarea", "Ev Sayısı", "Ortalama Lotarea", "Ortalama Fiyat");
      printf("\033[0m"); //Standart renkte bastırmak için
      break;
    case STREET:
    
      break;
    case NEIGHBORHOOD:
      
      break;
    case YEARBUILT:
      keeper = houses_head->yearbuilt;
      break;
    case OVERALLQUAL:
      
      break;
    
      break;
    case KITCHENQUAL:
      
      break;
    
    default:
      break;
    }

  while (houses_head->nextHouse != NULL) {
    switch (criter_name)
    {
    case LOTAREA:
    /*
    berkay-yildiz:
    Burada sonuçlarda anlamadığım şekilde hata var. Ortalama değerler çok anlamsız çıkıyor, neden olduğunu bulamadım.
    */
      tmp_i = houses_head->lotarea;
      if(tmp_i - keeper > criter_data) {
        printf("%-25d%-25d%-25d%-25d%-25d\n", keeper, tmp_i_old, counter, (avg_sum/counter), (price_sum/counter-1));
        price_sum = avg_sum = counter = 0;
        keeper = tmp_i;
      }
      price_sum += houses_head->saleprice;
      avg_sum += tmp_i;
      counter++;
      tmp_i_old = tmp_i;
      break;
    case STREET:
    printf("aka_street\n");
      strcpy(tmp_c_old, tmp_c);
      strcpy(tmp_c, houses_head->street);
      if(strcmp(tmp_c, tmp_c_old)) {
        printf("%s | %d\n", tmp_c_old, counter);
        counter = 1;
      } else counter ++;
      break;
    case NEIGHBORHOOD:
      strcpy(tmp_c_old, tmp_c);
      strcpy(tmp_c, houses_head->neighborhood);
      if(strcmp(tmp_c, tmp_c_old)) {
        printf("%s | %d\n", tmp_c_old, counter);
        counter = 1;
      } else counter ++;

      break;
    case YEARBUILT:
      if(houses_head->yearbuilt - keeper > criter_data) {
        printf("%d | %d\n", (avg_sum/counter), (price_sum/counter));
        price_sum = avg_sum = counter = 0;
        keeper = houses_head->yearbuilt;
      }
      price_sum += houses_head->saleprice;
      avg_sum += houses_head->yearbuilt;
      counter++;
      break;
    case OVERALLQUAL:
      tmp_i_old = tmp_i;
      tmp_i = houses_head->overallqual;
      if(tmp_i != tmp_i_old) {
        printf("%d | %d\n", tmp_i_old, counter);
        counter = 1;
      } else counter ++;

      break;
    case OVERALLCOND:
      tmp_i_old = tmp_i;
      tmp_i = houses_head->overallcond;
      if(tmp_i != tmp_i_old) {
        printf("%d | %d\n", tmp_i_old, counter);
        counter = 1;
      } else counter ++;
      break;
    case KITCHENQUAL:
      tmp_i_old = tmp_i;
      tmp_i = houses_head->kitchenqual;
      if(tmp_i != tmp_i_old) {
        printf("%d | %d\n", tmp_i_old, counter);
        counter = 1;
      } else counter ++;
      break;
    
    default:
    printf("hatali giris");
      break;
    }
    counter++;
    houses_head = houses_head->nextHouse;
  }
}

//link list olarak alınan evleri sıralayan fonksyon
House* sort_houses(House** houses, int criter_name){
  merge_sort(houses, criter_name);
  return *houses;
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

House* merge(House* in1, House* in2, int criter_name, int order){
  House* res = NULL;

  if (in1 == NULL) 
      return (in2); 
  else if (in2 == NULL) 
      return (in1);

  switch (criter_name)
  {
  case ID:
    
    if (order == ASC)
    {
      if (in1->id <= in2->id)
        {
          res = in1;
          res->nextHouse = merge(in1->nextHouse, in2, ID, order);
        } else
        {
          res = in2;
          res->nextHouse = merge(in1, in2->nextHouse, ID, order);
        }
        return (res);
    } else if (order == DESC)
    {
      if (in1->id >= in2->id)
        {
          res = in1;
          res->nextHouse = merge(in1->nextHouse, in2, ID, order);
        } else
        {
          res = in2;
          res->nextHouse = merge(in1, in2->nextHouse, ID, order);
        }
    return (res);
    }

    break;
  
  case LOTAREA:
       
    if (order == ASC)
    {
      if (in1->lotarea <= in2->lotarea)
        {
          res = in1;
          res->nextHouse = merge(in1->nextHouse, in2, LOTAREA, order);
        } else
        {
          res = in2;
          res->nextHouse = merge(in1, in2->nextHouse, LOTAREA, order);
        }
        return (res);
    } else if (order == DESC)
    {
      if (in1->lotarea >= in2->lotarea)
        {
          res = in1;
          res->nextHouse = merge(in1->nextHouse, in2, LOTAREA, order);
        } else
        {
          res = in2;
          res->nextHouse = merge(in1, in2->nextHouse, LOTAREA, order);
        }
    return (res);
    }
    
    break;
  
  case STREET:
    //String comparison
    if (order == ASC)
    {
      if (strcmp(in1->street, in2->street) <= 0)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, STREET, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, STREET, order);
      }
      return (res);
    } else if (order == DESC)
    {
      if (strcmp(in1->street, in2->street) >= 0)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, STREET, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, STREET, order);
      }
      return (res);
    }
    
    
    
    
    break;

  case SALEPRICE:
    if (order == ASC)
    {
      if (in1->saleprice <= in2->saleprice)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, SALEPRICE, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, SALEPRICE, order);
      }
      return (res);
    } else if (order == DESC)
    {
      if (in1->saleprice >= in2->saleprice)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, SALEPRICE, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, SALEPRICE, order);
      }
      return (res);
    }
    
    
    
    break;

  case NEIGHBORHOOD:
    //String comparison
    if (order == ASC)
    {
      if (strcmp(in1->neighborhood, in2->neighborhood) <= 0)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, NEIGHBORHOOD, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, NEIGHBORHOOD, order);
      }
      return (res);
    } else if (order == DESC)
    {
      if (strcmp(in1->neighborhood, in2->neighborhood) >= 0)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, NEIGHBORHOOD, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, NEIGHBORHOOD, order);
      }
      return (res);
    }
    
    
    
    break;
  
  case YEARBUILT:
    if (order == ASC)
    {
      if (in1->yearbuilt <= in2->yearbuilt)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, YEARBUILT, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, YEARBUILT, order);
      }
      return (res);
    } else if (order == DESC)
    {
      if (in1->yearbuilt >= in2->yearbuilt)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, YEARBUILT, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, YEARBUILT, order);
      }
      return (res);
    }
    
    
    
    break;
  
  case OVERALLQUAL:
    if (order == ASC)
    {
      if (in1->overallqual <= in2->overallqual)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, OVERALLQUAL, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, OVERALLQUAL, order);
      }
      return (res);
    } else if (order == DESC)
    {
        if (in1->overallqual >= in2->overallqual)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, OVERALLQUAL, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, OVERALLQUAL, order);
      }
      return (res);
    }
    
    
    
    break;

  case OVERALLCOND:
    if (order == ASC)
    {
      if (in1->overallcond <= in2->overallcond)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, OVERALLCOND, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, OVERALLCOND, order);
      }
      return (res);
    } else if (order == DESC)
    {
      if (in1->overallcond >= in2->overallcond)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, OVERALLCOND, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, OVERALLCOND, order);
      }
      return (res);
    }
    
    
    
    break;

  case KITCHENQUAL:
    if (order == ASC)
    {
      if (in1->kitchenqual <= in2->kitchenqual)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, KITCHENQUAL, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, KITCHENQUAL, order);
      }
      return (res);      
    } else if (order == DESC)
    {
      if (in1->kitchenqual >= in2->kitchenqual)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, KITCHENQUAL, order);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, KITCHENQUAL, order);
      }
      return (res);
    }
    
    
    
    break;

  default:
    //yanlış girdi girildiyse id'ye göre sıralar
    if (order == ASC)
    {
      if (in1->id <= in2->id)
        {
          res = in1;
          res->nextHouse = merge(in1->nextHouse, in2, ID, order);
        } else
        {
          res = in2;
          res->nextHouse = merge(in1, in2->nextHouse, ID, order);
        }
        return (res);
    } else if (order == DESC)
    {
      if (in1->id >= in2->id)
        {
          res = in1;
          res->nextHouse = merge(in1->nextHouse, in2, ID, order);
        } else
        {
          res = in2;
          res->nextHouse = merge(in1, in2->nextHouse, ID, order);
        }
    return (res);
    }
    break;
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

  //Listeyi ortadan ikiye bölüp first_node ve second_node pointerlarına atar
  split_list(head, &first_node, &second_node);
  
  switch (criter_name)
  {
  case ID: //ID
    merge_sort(&first_node, ID, order); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, ID, order);

    *list_head_ref = merge(first_node, second_node, ID, order); //Recursion bittikten sonra listeler birleştirilir
    break;
  
  case LOTAREA:
    merge_sort(&first_node, LOTAREA, order); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, LOTAREA, order);

    *list_head_ref = merge(first_node, second_node, LOTAREA, order); //Recursion bittikten sonra listeler birleştirilir
    break;
  
  case STREET:
    merge_sort(&first_node, STREET, order); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, STREET, order);

    *list_head_ref = merge(first_node, second_node, STREET, order); //Recursion bittikten sonra listeler birleştirilir
    break;

  case SALEPRICE:
    merge_sort(&first_node, SALEPRICE, order); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, SALEPRICE, order);

    *list_head_ref = merge(first_node, second_node, SALEPRICE, order); //Recursion bittikten sonra listeler birleştirilir
    break;

  case NEIGHBORHOOD:
    merge_sort(&first_node, NEIGHBORHOOD, order); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, NEIGHBORHOOD, order);

    *list_head_ref = merge(first_node, second_node, NEIGHBORHOOD, order); //Recursion bittikten sonra listeler birleştirilir
    break;
  
  case YEARBUILT:
    merge_sort(&first_node, YEARBUILT, order); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, YEARBUILT, order);

    *list_head_ref = merge(first_node, second_node, YEARBUILT, order); //Recursion bittikten sonra listeler birleştirilir
    break;
  
  case OVERALLQUAL:
    merge_sort(&first_node, OVERALLQUAL, order); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, OVERALLQUAL, order);

    *list_head_ref = merge(first_node, second_node, OVERALLQUAL, order); //Recursion bittikten sonra listeler birleştirilir
    break;

  case OVERALLCOND:
    merge_sort(&first_node, OVERALLCOND, order); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, OVERALLCOND, order);

    *list_head_ref = merge(first_node, second_node, OVERALLCOND, order); //Recursion bittikten sonra listeler birleştirilir
    break;

  case KITCHENQUAL:
    merge_sort(&first_node, KITCHENQUAL, order); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, KITCHENQUAL, order);

    *list_head_ref = merge(first_node, second_node, KITCHENQUAL, order); //Recursion bittikten sonra listeler birleştirilir
    break;

  default: //Eğer hiçbir değer girilmez ise ID ile sorting yapılır
    merge_sort(&first_node, ID, order); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, ID, order);

    *list_head_ref = merge(first_node, second_node, ID, order); //Recursion bittikten sonra listeler birleştirilir
    break;
  }
  
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