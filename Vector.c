#include "Vector.h"
#include <stdlib.h>

/**
 * Allocates dynamically new vector .
 * @param elem_copy_func func which copies the element stored in the vector (returns
 * dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in the vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
Vector *VectorAlloc(VectorElemCpy elem_copy_func, VectorElemCmp elem_cmp_func, VectorElemFree elem_free_func){
    if (elem_cmp_func == NULL || elem_copy_func == NULL || elem_free_func == NULL){
        return NULL;
    }
    Vector * ret_vec = malloc(sizeof(Vector));
    if (ret_vec == NULL){ return NULL;}
    ret_vec->capacity = VECTOR_INITIAL_CAP;
    ret_vec->data = malloc( ret_vec->capacity * sizeof(void *));
    if (ret_vec->data == NULL){
        free(ret_vec);
        return NULL;
    }
    ret_vec->elem_free_func = elem_free_func;
    ret_vec->elem_copy_func = elem_copy_func;
    ret_vec->elem_cmp_func = elem_cmp_func;
    ret_vec->size = 0;
    return  ret_vec;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void VectorFree(Vector **p_vector) {
    if (p_vector == NULL || *p_vector == NULL) {
        return;
    }
    if ((*p_vector)->data != NULL) { // can happen on failed realloc.
        for (size_t ix = 0; ix < (*p_vector)->size; ix++) {
            // clarification, vector allocates every element in it.
            (*p_vector)->elem_free_func(&(*p_vector)->data[ix]);
            }
        free((*p_vector)->data);
    }
    free(*p_vector);
    *p_vector = NULL;
}

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element the given index if exists (the element itself, not a copy of it)
 * , NULL otherwise.
 */
void *VectorAt(Vector *vector, size_t ind){
    if ((vector == NULL) || (vector->data == NULL) || (ind >= vector->size)){//size_t is UL ad so no need to check negatives
        return  NULL;
    }
    void* ret =  vector->data[ind];
    return ret;
}

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the
 * vector ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int VectorFind(Vector *vector, void *value){
    //vector is not sorted and so linear search is the optimal search.
    if (vector == NULL || value == NULL){
        return -1;
    }
    for (size_t ix = 0 ; ix < vector->size ; ix++){
        if(vector->elem_cmp_func(VectorAt(vector, ix), value)){
            return ix;
        }
    }
    return  -1;
}

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int VectorPushBack(Vector *vector, void *value){
    if (vector == NULL || value == NULL){
        return 0;
    }
    vector->size++; // Check LF with new size.
    if (VectorGetLoadFactor(vector) > VECTOR_MAX_LOAD_FACTOR){
        vector->capacity *= VECTOR_GROWTH_FACTOR;
        vector->data = realloc(vector->data, vector->capacity * sizeof(void*));
        if (vector->data == NULL){
            VectorFree(&vector);
            return 0;
        }
    }
    vector->data[vector->size - 1] =  vector->elem_copy_func(value);
    return 1;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double VectorGetLoadFactor(Vector *vector){
    if (vector == NULL || vector->capacity == 0){
        return -1; // we may assume that vector->capacity is never 0 but still..
    }
    return (double) vector->size/vector->capacity;

}

/**
 * Removes the element at the given index from the vector.
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int VectorErase(Vector *vector, size_t ind){
    if (vector == NULL || ind >= vector->size){ return 0;}
    void * elem_to_free = VectorAt(vector,ind);
    vector->elem_free_func(&elem_to_free);
    vector->size--;
    for (size_t ix = ind ; ix < vector->size ; ix ++){
        vector->data[ix] = vector->data[ix + 1]; // order of insertion is important
    }
    if (VectorGetLoadFactor(vector) < VECTOR_MIN_LOAD_FACTOR){
        vector->capacity /= VECTOR_GROWTH_FACTOR;
        vector->data = realloc(vector->data, vector->capacity * sizeof(void*));
        if (vector->data == NULL){
            VectorFree(&vector);
            return 0;
        }
    }
    return 1;
}

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void VectorClear(Vector *vector){ //to save lgn operation i will not use VectorErase.
    if (vector == NULL){
        return;
    }
    for (size_t ix = 0; ix < vector->size ; ix ++ ){
        vector->elem_free_func(&vector->data[ix]);
        vector->data[ix] = NULL;
    }
    vector->size = 0;
    vector->capacity = 2; // should it be initial capacity?
    vector->data = realloc(vector->data, vector->capacity * sizeof(void*));
    if (vector->data == NULL) {
        VectorFree(&vector);
    }
}
