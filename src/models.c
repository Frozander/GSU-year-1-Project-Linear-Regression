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

void create_data_matrices(House* houses,Matrix* X,Matrix* Y){
  printf("Create data matrices from dataset\n");
  House* tmp = houses;
  Matrix* X_tmp = create_matrix(1360, 2);
  Matrix* Y_tmp = create_matrix(1360, 1);

  // Self Note: Get the size of linked list
  //            create matrices
  //            Iterate through the linked list
  //            Write lotarea and price to respective matrices
  int k = 0;
  
  while (tmp != NULL)
  {
    X_tmp->values[k][0] = 1;
    X_tmp->values[k][1] = tmp->saleprice;
    
    Y_tmp->values[k][0] = tmp->lotarea; 
    
    tmp = tmp->nextHouse;

    k++;
  }

  X = X_tmp;
  Y = Y_tmp;  
  
  return;
}

Matrix* get_transpose(Matrix* A){
  Matrix* Atranspose;
  printf("Get Transpose\n");
  Atranspose = create_matrix(A->collmuns, A->lines);
  for (size_t i = 0; i < A->collmuns; i++)
  {
    for (size_t j = 0; j < A->lines; j++)
    {
      Atranspose->values[i][j] = A->values[j][i];
    }
  }
  return Atranspose;
}


Matrix* get_inverse(Matrix* A){
  Matrix* Ainverse;
  printf("Get inverse\n");
  /*  Frozander
    Inverse matrix sadece 2x2 fonksiyonlarda olduğu için
    onlara özel bir yöntem kullanılacak
    */
  Ainverse = create_matrix(2, 2);
  // Calculate teh determinant of the function
  double det = A->values[0][0] * A->values[1][1] - A->values[1][0] * A->values[0][1];
  Ainverse->values[0][0] =  A->values[1][1] / det;
  Ainverse->values[1][0] = -A->values[1][0] / det;
  Ainverse->values[0][1] = -A->values[0][1] / det;
  Ainverse->values[1][1] =  A->values[0][0] / det;

  return Ainverse;
}


Matrix* get_multiplication(Matrix* A, Matrix* B){
  Matrix* C;
  printf("Multiplication\n");
  C = create_matrix(A->lines, B->collmuns);
  for (size_t i = 0; i < B->collmuns; i++)
  {
    for (size_t j = 0; j < A->lines; j++)
    {
      C->values[i][j] = 0;
      for (size_t k = 0; k < A->lines; k++)
      {
        C->values[i][j] += A->values[k][i] * B->values[j][k];
      }
    }
  }
  
  return C;
}


Matrix* calculate_parameter(House* houses, Matrix* X, Matrix* Y){
  Matrix* W;
  printf("Calculate parameters for dataset\n");
  create_data_matrices(houses, X, Y);
  Matrix* X_transpose = get_transpose(X);
  Matrix* tmp_matrix = get_multiplication(X_transpose, X);
  Matrix* tmp_matrix_2 = get_multiplication(X_transpose, Y);
  W = get_multiplication(tmp_matrix, tmp_matrix_2);
  free(X_transpose);
  free(tmp_matrix);
  free(tmp_matrix_2);
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



