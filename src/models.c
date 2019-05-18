#include"models.h"
#include"dataset.h"
#include<stdio.h>


int model_by_similarity(House * houses [], House  * house) {
  House * head;
  int avg;
  int gap;

  head = get_neighborhoods(house, houses);
  printf("\nKomsular alindi\nKomsu sayisi: %d\n", get_list_lenght(head));

  gap = 2000;
  limit_houses(&head, LOTAREA, (house->lotarea - gap), (house->lotarea + gap));
  printf("\nLotarea ya göre limitlendi\nEv sayisi: %d\n", get_list_lenght(head));

  gap = 5;
  limit_houses(&head, YEARBUILT, (house->yearbuilt - gap), (house->yearbuilt + gap));
  printf("\nYearbuilt e göre limitlendi\nEv sayisi: %d\n", get_list_lenght(head));

  return get_criter_avg(head, SALEPRICE);
}



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


Matrix* create_matrix(int line, int collumn){
  // Yeni bir iki boyutlu array (Matrix) oluşturulur
  Matrix* new_matrix = malloc(sizeof(Matrix));
  // Daha sonra kullanmak için boyutu satır ve sütun şeklinde saklayan değerler atanır
  new_matrix->lines = line;
  new_matrix->collmuns = collumn;
  // Verilerin tutulduğu values double** iki boyutlu arrayinin 1. boyutu oluşturulur (satır)
  new_matrix->values = malloc(line * sizeof(double *));
  //For düngüsü ile 1. boyuttaki tüm değerler için sütunlar oluşturulur
  for (size_t i = 0; i < new_matrix->lines; i++)
  {
    new_matrix->values[i] = malloc(collumn * sizeof(double));
    //ikinci bir for döngüsü ile arraydeki tüm veriler sıfırlanır
    for (size_t j = 0; j < collumn; j++)
    {
      new_matrix->values[i][j] = 0;
    }
  }
  return new_matrix;
}

void free_matrix(Matrix* matrix_in){
  for (size_t i = 0; i < matrix_in->lines; i++)
  {
    free(matrix_in->values[i]);
  }
  free(matrix_in);
}

void print_matrix(Matrix* matrix_in){
  printf("Printing Matrix\n");
  for (size_t i = 0; i < matrix_in->lines; i++)
  {
    for (size_t j = 0; j < matrix_in->collmuns; j++)
    {
      printf("%.2lf ", matrix_in->values[i][j]);
    }
    printf("\n");
  }
}

void create_data_matrices(House** houses,Matrix** X,Matrix** Y){
  House* tmp = *houses;
  int counter = 0;
  House* cur = *houses;
  while (cur!=NULL)
  {
    cur= cur->nextHouse;
    counter++;
  }
  Matrix* X_tmp = create_matrix(counter, 2);
  Matrix* Y_tmp = create_matrix(counter, 1);
  int k = 0;
  while (tmp != NULL)
  {
    //printf("\n%d %d", tmp->lotarea, tmp->saleprice);
    X_tmp->values[k][0] = 1;
    X_tmp->values[k][1] = tmp->lotarea;
    
    Y_tmp->values[k][0] = tmp->saleprice; 
    
    tmp = tmp->nextHouse;
    k++;
  }

  (*X) = X_tmp;
  (*Y) = Y_tmp;  
  
  return;
}

Matrix* get_transpose(Matrix* A){
  Matrix* Atranspose;
  Atranspose = create_matrix(A->collmuns, A->lines);
  for (size_t i = 0; i < A->lines; i++)
  {
    for (size_t j = 0; j < A->collmuns; j++)
    {
      Atranspose->values[j][i] = A->values[i][j];
    }
  }
  return Atranspose;
}

Matrix* get_inverse(Matrix* A){
  Matrix* Ainverse;
  /*  Frozander
    Inverse matrix sadece 2x2 fonksiyonlarda olduğu için
    onlara özel bir yöntem kullanılacak
    */
  Ainverse = create_matrix(2, 2);
  // Calculate teh determinant of the function
  double det = A->values[0][0] * A->values[1][1] - A->values[1][0] * A->values[0][1];
  Ainverse->values[0][0] =  A->values[1][1] / det;
  Ainverse->values[1][0] = -1 * A->values[1][0] / det;
  Ainverse->values[0][1] = -1 *A->values[0][1] / det;
  Ainverse->values[1][1] =  A->values[0][0] / det;
  return Ainverse;
}

Matrix* get_multiplication(Matrix* A, Matrix* B){
  Matrix* C;
  C = create_matrix(A->lines, B->collmuns);
  for (size_t i = 0; i < C->lines; i++)
  {
    for (size_t j = 0; j < C->collmuns; j++)
    {
      C->values[i][j] = 0;
      for (size_t k = 0; k < A->collmuns; k++)
      {
        C->values[i][j] += A->values[i][k] * B->values[k][j];
      }
    }
  }
  return C;
}

Matrix* calculate_parameter(House* houses){
  Matrix* W;
  Matrix* X;
  Matrix* Y;
  create_data_matrices(&houses, &X, &Y);
  Matrix* X_transpose = get_transpose(X);
  Matrix* tmp_matrix = get_multiplication(X_transpose, X);
  Matrix* tmp_matrix_2 = get_multiplication(X_transpose, Y);
  Matrix* tmp_matrix_inv = get_inverse(tmp_matrix);
  W = get_multiplication(tmp_matrix_inv, tmp_matrix_2);
  free_matrix(X_transpose);
  free_matrix(tmp_matrix);
  free_matrix(tmp_matrix_2);
  free_matrix(X);
  free_matrix(Y);
  free_matrix(tmp_matrix_inv);
  return W;
}

Matrix* make_prediction(House** house_in,Matrix* W){
  Matrix* predicted_prices;
  printf("Make prediction\n");
  // TODO
  // 1 - filename olarak verilen test verisini oku,
  //   yeni houses dizisi olustur
  // 2 - create_data_matrices kullanarak X ve y matrislerini olustur
  // 3 - Daha onceden hesaplanan W parametresini kullanarak
  //  fiyat tahmini yap, burda yapilmasi gereken
  //  X ve W matrislerinin carpimini bulmak
  // 4 - Sonuclari bir dosyaya yaz
  //print_house(linear_houses, MULTI, 0);
  House* tmp = *house_in;
  int counter = 0;
  House* cur = *house_in;
  
  while (cur!=NULL)
  {
    counter++;
    cur = cur->nextHouse;
  }
  
  Matrix* X= create_matrix(counter, 2);
  int k = 0;
  
  while (tmp != NULL)
  {
    X->values[k][0] = 1;
    X->values[k][1] = tmp->lotarea;
    tmp = tmp->nextHouse;
    k++;
  } 
  
  predicted_prices = get_multiplication(X, W);
  free_matrix(X);
  
  return predicted_prices;
}

void file_write_house(House** house_in, char* filename){
  FILE* stream = fopen(filename, "w");

  fprintf(stream, "id,lotarea,street,saleprice,neighborhood,yearbuilt,overallqual,overallcond,kitchenqual");
  
  House* cursor = *house_in;
  while (cursor != NULL)
  {
    fprintf(stream,
            "%d,%d,%s,%d,%s,%d,%d,%d,%s\n", //Alınan evin verisini yazdırıyoruz
            cursor->id,
            cursor->lotarea,
            cursor->street,
            cursor->saleprice,
            cursor->neighborhood,
            cursor->yearbuilt,
            cursor->overallqual,
            cursor->overallcond,
            convert_kitchenqual_back(cursor->kitchenqual)
          );
    cursor = cursor->nextHouse;
  }

  fclose(stream);
  
}

void matrix_to_house_list(Matrix* matrix_in, House** house_out){
  //Error handling
  if (matrix_in->collmuns != 1)
  {
    printf("\nNot a final matrix!");
    return;
  }
  if (house_out == NULL)
  {
    printf("\nOutput list is empty");
    return;
  }

  House* cursor = *house_out;
  int i = 0;

  while (cursor->nextHouse != NULL)
  {
    cursor->saleprice = matrix_in->values[i][0];
    cursor = cursor->nextHouse;
    i++;
  }
  
}