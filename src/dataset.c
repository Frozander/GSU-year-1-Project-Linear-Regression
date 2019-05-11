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
  
  if(houses[hashIndex] != NULL) { //verilen hash değeri için tablodaki pointer NULL mu bakıyoruz
    tmp_house = houses[hashIndex]; //NULL değilse adresi geçici bir pointera alıyoruz
    if (tmp_house->id == id) return *tmp_house; //elimizdeki adresteki değer istediğimiz id ile eşleşiyor mu bakıyoruz eşleşiyorsa döndürüyoruz
    while (tmp_house->nextHouse != NULL) { //eşleşmiyorsa sonraki adres varsa oraya bakıyoruz
      tmp_house = tmp_house->nextHouse;
      if (tmp_house->id == id) return *tmp_house;
    }
  }

  /* berkay-yildiz:
    eğer tabloda olmayan bir id girilirse pointerda kalan adres geri dönüyor, çok önemli sayılmaz ama sona doğru halledilmeli
   */

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

House* merge(House* in1, House* in2, int style){
  House* res = NULL;

  if (in1 == NULL) 
      return (in2); 
  else if (in2 == NULL) 
      return (in1);

  switch (style)
  {
  case ID:

    if (in1->id <= in2->id)
    {
      res = in1;
      res->nextHouse = merge(in1->nextHouse, in2, ID);
    } else
    {
      res = in2;
      res->nextHouse = merge(in1, in2->nextHouse, ID);
    }
    return (res);

    break;
  
  case LOTAREA:
    
    if (in1->id <= in2->id)
    {
      res = in1;
      res->nextHouse = merge(in1->nextHouse, in2, ID);
    } else
    {
      res = in2;
      res->nextHouse = merge(in1, in2->nextHouse, ID);
    }
    return (res);
    
    break;
  
  case STREET:
    //String comparison
    
    

    if (strcmp(in1->street, in2->street) <= 0)
    {
      res = in1;
      res->nextHouse = merge(in1->nextHouse, in2, STREET);
    } else
    {
      res = in2;
      res->nextHouse = merge(in1, in2->nextHouse, STREET);
    }
    return (res);
    
    break;

  case SALEPRICE:
    
    if (in1->saleprice <= in2->saleprice)
    {
      res = in1;
      res->nextHouse = merge(in1->nextHouse, in2, SALEPRICE);
    } else
    {
      res = in2;
      res->nextHouse = merge(in1, in2->nextHouse, SALEPRICE);
    }
    return (res);
    
    break;

  case NEIGHBORHOOD:
    //String comparison
    if (strcmp(in1->neighborhood, in2->neighborhood) <= 0)
    {
      res = in1;
      res->nextHouse = merge(in1->nextHouse, in2, NEIGHBORHOOD);
    } else
    {
      res = in2;
      res->nextHouse = merge(in1, in2->nextHouse, NEIGHBORHOOD);
    }
    return (res);
    
    break;
  
  case YEARBUILT:
    
    if (in1->yearbuilt <= in2->yearbuilt)
    {
      res = in1;
      res->nextHouse = merge(in1->nextHouse, in2, YEARBUILT);
    } else
    {
      res = in2;
      res->nextHouse = merge(in1, in2->nextHouse, YEARBUILT);
    }
    return (res);
    
    break;
  
  case OVERALLQUAL:
    
    if (in1->overallqual <= in2->overallqual)
    {
      res = in1;
      res->nextHouse = merge(in1->nextHouse, in2, OVERALLQUAL);
    } else
    {
      res = in2;
      res->nextHouse = merge(in1, in2->nextHouse, OVERALLQUAL);
    }
    return (res);
    
    break;

  case OVERALLCOND:
    
    if (in1->overallcond <= in2->overallcond)
    {
      res = in1;
      res->nextHouse = merge(in1->nextHouse, in2, OVERALLCOND);
    } else
    {
      res = in2;
      res->nextHouse = merge(in1, in2->nextHouse, OVERALLCOND);
    }
    return (res);
    
    break;

  case KITCHENQUAL:
    
    if (in1->kitchenqual <= in2->kitchenqual)
    {
      res = in1;
      res->nextHouse = merge(in1->nextHouse, in2, KITCHENQUAL);
    } else
    {
      res = in2;
      res->nextHouse = merge(in1, in2->nextHouse, KITCHENQUAL);
    }
    return (res);
    
    break;

  default:
    //yanlış girdi girildiyse id'ye göre sıralar
    if (in1->id <= in2->id)
      {
        res = in1;
        res->nextHouse = merge(in1->nextHouse, in2, ID);
      } else
      {
        res = in2;
        res->nextHouse = merge(in1, in2->nextHouse, ID);
      }
      return (res);

    break;
  }
    
}

House* merge_sort(House ** list_head_ref, int style){

  House* head = *list_head_ref;
  House* first_node;
  House* second_node;

  if ((head == NULL) || (head->nextHouse == NULL)) //Eğer liste boşsa veya bir elemanı varsa foksiyondan çıkar
  {
    return;
  }

  //Listeyi ortadan ikiye bölüp first_node ve second_node pointerlarına atar
  split_list(list_head_ref, first_node, second_node);
  
  switch (style)
  {
  case ID: //ID
    merge_sort(&first_node, ID); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, ID);

    *list_head_ref = merge(first_node, second_node, ID); //Recursion bittikten sonra listeler birleştirilir
    break;
  
  case LOTAREA:
    merge_sort(&first_node, LOTAREA); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, LOTAREA);

    *list_head_ref = merge(first_node, second_node, LOTAREA); //Recursion bittikten sonra listeler birleştirilir
    break;
  
  case STREET:
    merge_sort(&first_node, STREET); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, STREET);

    *list_head_ref = merge(first_node, second_node, STREET); //Recursion bittikten sonra listeler birleştirilir
    break;

  case SALEPRICE:
    merge_sort(&first_node, SALEPRICE); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, SALEPRICE);

    *list_head_ref = merge(first_node, second_node, SALEPRICE); //Recursion bittikten sonra listeler birleştirilir
    break;

  case NEIGHBORHOOD:
    merge_sort(&first_node, NEIGHBORHOOD); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, NEIGHBORHOOD);

    *list_head_ref = merge(first_node, second_node, NEIGHBORHOOD); //Recursion bittikten sonra listeler birleştirilir
    break;
  
  case YEARBUILT:
    merge_sort(&first_node, YEARBUILT); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, YEARBUILT);

    *list_head_ref = merge(first_node, second_node, YEARBUILT); //Recursion bittikten sonra listeler birleştirilir
    break;
  
  case OVERALLQUAL:
    merge_sort(&first_node, OVERALLQUAL); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, OVERALLQUAL);

    *list_head_ref = merge(first_node, second_node, OVERALLQUAL); //Recursion bittikten sonra listeler birleştirilir
    break;

  case OVERALLCOND:
    merge_sort(&first_node, OVERALLCOND); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, OVERALLCOND);

    *list_head_ref = merge(first_node, second_node, OVERALLCOND); //Recursion bittikten sonra listeler birleştirilir
    break;

  case KITCHENQUAL:
    merge_sort(&first_node, KITCHENQUAL); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, KITCHENQUAL);

    *list_head_ref = merge(first_node, second_node, KITCHENQUAL); //Recursion bittikten sonra listeler birleştirilir
    break;

  default: //Eğer hiçbir değer girilmez ise ID ile sorting yapılır
    merge_sort(&first_node, ID); //Her iki node için de recursive merge_sort çağırılır.
    merge_sort(&second_node, ID);

    *list_head_ref = merge(first_node, second_node, ID); //Recursion bittikten sonra listeler birleştirilir
    break;
  }

  return head;
  
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