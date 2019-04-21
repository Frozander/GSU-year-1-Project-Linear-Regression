#include"models.h"
#include"dataset.h"
#include<stdio.h>

int model_by_similarity(House* houses,House new_house){
  printf("Find price for house %d\n",new_house.id);
  int price;
  //TODO

  // 1 - Oncelikle ayni komsuluktaki evleri bulun
  // 2 - Bu evleri lotArea ya gore siralayin
  // 3 - new_house degiskenin lotarea parametresine en
  //  yakin evleri alin, bu evlerin alanlari 
  //  (new_house.lotarea+2000) ve (new_house.lotarea-2000) metrekare arasinda
  //   olabilir.
  // 4 - Kalan evleri yillarina gore siralayin
  // 5 - new_house degiskenin yearbuilt parametresine en yakin
  // evleri secin, bu evlerin yapim tarihleri
  //  (new_house.yearbuilt+5) ve (new_house-5) arasinda olabilir.
  // 6 - Ek olarak kaliteye gore secim yapabilirsiniz.
  // 7 - Son elemeden sonra elinizde kalan evlerin fiyat ortalamasini alin
  // 8 - Yeni gelen ev icin fiyat degeri bu ortalama olmalidir.
  
  return price;
}



void create_data_matrices(House* houses,int** X,int* y){
  printf("Create data matrices from dataset\n");
  // TODO
  return;
}

int** get_transpose(int** A){
  int ** Atranspose; 
  printf("Get Transpose\n");
  // TODO
  return Atranspose;
}


int** get_inverse(int** A){
  int** Ainverse;
  printf("Get inverse\n");
  // TODO
  return Ainverse;
}


int** get_multiplication(int** A, int** B){
  int ** C;
  printf("Multiplication\n");
  // TODO
  return C;
}


int** calculate_parameter(int** X, int* y){
  int** W;
  printf("Calculate parameters for dataset\n");
  // TODO
  return W;

}

int** make_prediction(char* filename,int** W){
  int** predicted_prices;
  printf("Make prediction\n");
  // TODO
  // 1 - filename olarak verilen test verisini oku,
  //   yeni houses dizisi olustur
  // 2 - create_data_matrices kullanarak X ve y matrislerini olustur
  // 3 - Daha onceden hesaplanan W parametresini kullanarak
  //  fiyat tahmini yap, burda yapilmasi gereken
  //  X ve W matrislerinin carpimini bulmak
  // 4 - Sonuclari bir dosyaya yaz

}



