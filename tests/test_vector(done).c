#include <stdio.h>
#include <assert.h>
#include "Vector.h"
#include "Pair.h"
#include "HashMap.h"
#include "PairCharInt.h"
#include "Hash.h"

void *ElemCpy(const void *elem) {
  int *a = malloc(sizeof(int));
  *a = *((int *) elem);
  return a;
}
int ElemCmp(const void *elem_1, const void *elem_2) {
  return *((const int *) elem_1) == *((const int *) elem_2);
}
void ElemFree(void **elem) {
  free(*elem);
}

typedef void (*VectorElemPrint)(void *);

void ElemPrintINT(void *elem)
{
  printf("%d, ", *(int *)elem);
}

void PrintVector(Vector *p_vector, VectorElemPrint elem_print) {
  for (size_t elem_i = 0; elem_i < p_vector->size; ++elem_i) {
    elem_print(p_vector->data[elem_i]);
  }
  printf("\n");
}

void Test2(){

  Vector *vector = VectorAlloc(ElemCpy, ElemCmp, ElemFree);
  for (int k_i = 0; k_i < 18; ++k_i) {
    VectorPushBack(vector, &k_i);
  }

  PrintVector(vector, ElemPrintINT);
  assert(*(int *)VectorAt(vector, 3) == 3 && "Didn't find the right spot");
  assert(*(int *)VectorAt(vector, 17) == 17 && "Outside of the vector");
  assert(VectorAt(vector, 19) == NULL && "Outside of the vector");
  assert(VectorAt(vector, -9) == NULL && "Outside of the vector");

  int p = 5;

  assert(VectorFind(vector, &p) == 5 && "Didn't find the right spot");
  VectorErase(vector, 3);

  assert(vector->size == 17);
  assert(vector->capacity == 32);

  PrintVector(vector, ElemPrintINT);

  for (int k_i = 0; k_i < 8; ++k_i) {
    VectorErase(vector, 0);
  }

  assert(vector->size == 9);
  assert(vector->capacity == 32);
  PrintVector(vector, ElemPrintINT);

  for (int k_i = 0; k_i < 3; ++k_i) {
    VectorErase(vector, 0);
  }

  assert(vector->size == 6);
  assert(vector->capacity == 16);

  VectorClear(vector);
  assert(vector->size == 0);
  assert(vector->capacity == 2);

  VectorPushBack(vector, &p);
  assert(vector->size == 1);
  assert(vector->capacity == 2);
  assert(VectorGetLoadFactor(vector) == 0.5);

  PrintVector(vector, ElemPrintINT); // should be: 5,

  for (int k_i = 0; k_i < 8; ++k_i) {
    VectorPushBack(vector, &k_i);
  }

  assert(vector->capacity == 16);
  assert(vector->size == 9);

  PrintVector(vector, ElemPrintINT); // should be: 5, 0, 1, 2, 3, 4, 5, 6, 7,

  VectorFree(&vector);
}

void Test1(){
  Vector *vector = VectorAlloc(ElemCpy, ElemCmp, ElemFree);
  for (int k_i = 0; k_i < 8; ++k_i) {
    VectorPushBack(vector, &k_i);
  }

  PrintVector(vector, ElemPrintINT); // should be: 0, 1, 2, 3, 4, 5, 6, 7,
  assert(*(int *)VectorAt(vector, 3) == 3 && "Didn't find the right spot");
  assert(VectorAt(vector, 9) == NULL && "Outside of the vector");
  assert(VectorAt(vector, -9) == NULL && "Outside of the vector");

  int p = 5;

  assert(VectorFind(vector, &p) == 5 && "Didn't find the right spot");
  VectorErase(vector, 3);

  assert(vector->size == 7);
  assert(vector->capacity == 16UL);

  PrintVector(vector, ElemPrintINT); // should be: 0, 1, 2, 4, 5, 6, 7,

  VectorClear(vector);
  PrintVector(vector, ElemPrintINT);
  assert(vector->size == 0);
  assert(vector->capacity == 2);

  VectorPushBack(vector, &p);
  assert(vector->size == 1);
  assert(vector->capacity == 2);
  assert(VectorGetLoadFactor(vector) == 0.5);

  PrintVector(vector, ElemPrintINT); // should be: 5,

  for (int k_i = 0; k_i < 8; ++k_i) {
    VectorPushBack(vector, &k_i);
  }

  assert(vector->capacity == 16);
  assert(vector->size == 9);

  PrintVector(vector, ElemPrintINT); // should be: 5, 0, 1, 2, 3, 4, 5, 6, 7,

  VectorFree(&vector);
}

void Test3(){
  Vector *vector = VectorAlloc(ElemCpy, ElemCmp, ElemFree);
  for (int k_i = 0; k_i < 3; ++k_i) {
    VectorPushBack(vector, &k_i);
  }

  VectorClear(vector);
  assert(vector->size == 0);
  assert(vector->capacity == 2);

  VectorFree(&vector);

  vector = VectorAlloc(ElemCpy, ElemCmp, ElemFree);
  for (int k_i = 0; k_i < 2; ++k_i) {
    VectorPushBack(vector, &k_i);
  }

  VectorClear(vector);
  assert(vector->size == 0);
  assert(vector->capacity == 4);

  VectorFree(&vector);

  vector = VectorAlloc(ElemCpy, ElemCmp, ElemFree);
  for (int k_i = 0; k_i < 1; ++k_i) {
    VectorPushBack(vector, &k_i);
  }

  VectorClear(vector);
  assert(vector->size == 0);
  assert(vector->capacity == 8);

  VectorFree(&vector);
}

int main() {
  //vector tests
  Test1();
  Test2();
  Test3();

  printf("\nPASSED ALL TESTS!!\n");
  return 0;
}

