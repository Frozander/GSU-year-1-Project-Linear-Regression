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

Matrix* create_matrix(int line, int collumn){
  // Yeni bir iki boyutlu array (Matrix) oluşturulur
  Matrix* new_matrix = malloc(sizeof(Matrix));
  // Daha sonra kullanmak için boyutu satır ve sütun şeklinde saklayan değerler atanır
  new_matrix->lines = line;
  new_matrix->collmuns = collumn;
  // Verilerin tutulduğu values double** iki boyutlu arrayinin 1. boyutu oluşturulur (satır)
  new_matrix->values = malloc(sizeof(double*) * line);
  //For düngüsü ile 1. boyuttaki tüm değerler için sütunlar oluşturulur
  for (size_t i = 0; i < new_matrix->collmuns; i++)
  {
    new_matrix->values[i] = malloc(sizeof(double) * collumn);
    //ikinci bir for döngüsü ile arraydeki tüm veriler sıfırlanır
    for (size_t j = 0; j < collumn; j++)
    {
      new_matrix->values[i][j] = 0;
    }
  }
  return new_matrix;
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
  /*  Frozander
    Inverse matrix sadece 2x2 fonksiyonlarda olduğu için
    onlara özel bir yöntem kullanılacak
    */
  return Ainverse;
}


int** get_multiplication(int** A, int** B){
  int ** C;
  printf("Multiplication\n");
  // TODO
  return C;
}


int** calculate_parameter(House* houses){
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



