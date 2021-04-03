#include "HashMap.h"

/**
 *
 * -------------------------------------------------------------------------------------------------------------------
 *  my addition
 *  helper functions.
 */

/**
 * will be used in Clear and in rehash to avoid doubles
 * @param hash_map
 */
void HashMapClearNoCapacity(HashMap *hash_map){
    if ( NULL == hash_map){return;}
    for (size_t ix = 0 ; ix <hash_map->capacity ; ix ++){
        VectorClear(hash_map->buckets[ix]);
    }
    hash_map->size = 0;

}



/**
 *  return a hash that is appropriate to the actual map
 * @param hashmap
 * @param key
 * @return
 */
int AdditionalHash(HashMap *hashmap, KeyT key){
    size_t first_hash = hashmap->hash_func(key);
    size_t ret = (first_hash & (hashmap->capacity -1));
    return ret;
}


/**
 * helper for buckets copy
 * @param vec
 * @return
 */

Vector * VectorCopy( Vector* vec){
    Vector * ret_vec = VectorAlloc(vec->elem_copy_func, vec->elem_cmp_func, vec->elem_free_func);
    ret_vec->capacity = vec->capacity;
    ret_vec->size = vec->size;
    for (size_t ix = 0 ; ix < vec->size; ix++){
        void *curr = vec->elem_copy_func(vec->data[ix]);
        VectorPushBack(ret_vec, curr);
    }
    return ret_vec;
}

/**
 * will be used for rehashing.
 * @important!!!!!! everything that has been allocated here will be freed with BucketsFree in the function the called this.
 * @param buckets buckets to copy
 * @param old_cap old capacity to know how many vectors are there
 * @return a pointer to a copy of buckets
 */
Vector ** CopyBuckets(Vector ** buckets, size_t old_cap){
        Vector ** ret_val = calloc(old_cap, sizeof(Vector*));
        for (size_t ix = 0; ix < old_cap ; ix++){
            ret_val[ix] = VectorCopy(buckets[ix]);
        }
        return ret_val;
}

/**
 * frees a given array of buckets (vectors)
 * @param p_buckets pointer to buckets (buckets is (**Vector) and so p_buckets is (***Vector))
 * @param capacity
 */
void BucketsFree(Vector *** p_buckets, size_t capacity){
    if (p_buckets == NULL || *p_buckets == NULL){
        return;
    }
    for (size_t ix = 0 ; ix < capacity ; ix++){
        VectorFree(&(*p_buckets)[ix]);
    }
    free(*p_buckets);
    *p_buckets = NULL;

}


/**
 * takes a vector and hashes its pairs to HashMap
 * @param hashmap
 * @param vec
 */
void RehashSingleVector(HashMap * hashmap, Vector * vec){
    for (size_t ix = 0 ; ix < vec->size ; ix++){
        HashMapInsert(hashmap, vec->data[ix]);
    }
}
/**
 * this function rehash only the pairs in the vectors that are going to be dismissed
 * @param hashmap
 * @param old_buckets - a copy !!!!!!
 * @param old_capacity
 */
void DownwardsRehash(HashMap * hashmap, Vector ** old_buckets, size_t old_capacity){
    HashMapClearNoCapacity(hashmap);
    for (size_t ix = hashmap->capacity ; ix < old_capacity ; ix++){
        RehashSingleVector(hashmap, old_buckets[ix]);
    }
}



/**
 *  this function rehash every single pair in old buckets to new buckets
 * @param hashmap
 * @param old_buckets - a copy !!!!!!
 * @param old_capacity
 */
void UpwardsRehash(HashMap * hashmap, Vector ** old_buckets, size_t old_capacity){
    HashMapClearNoCapacity(hashmap);
    for (size_t ix = 0 ; ix < old_capacity ; ix++){
        RehashSingleVector(hashmap, old_buckets[ix]);
    }
}

/**
 * -------------------------------------------------------------------------------------------------------------------
 * HashMap.h implementation.
 */

/**
 * Allocates dynamically new hash map element.
 * @param hash_func a function which "hashes" keys.
 * @param pair_cpy a function which copies pairs.
 * @param pair_cmp a function which compares pairs.
 * @param pair_free a function which frees pairs.
 * @return pointer to dynamically allocated HashMap.
 * @if_fail return NULL.
 */
HashMap *HashMapAlloc(HashFunc hash_func, HashMapPairCpy pair_cpy, HashMapPairCmp pair_cmp, HashMapPairFree pair_free){
    if (NULL == hash_func || NULL == pair_cmp  || NULL == pair_cpy || NULL == pair_free){
        return NULL;
    }
    HashMap *hm = malloc(sizeof(HashMap));
    if (  NULL == hm ){return NULL;}
    hm->capacity = HASH_MAP_INITIAL_CAP;
    hm->buckets = malloc(hm->capacity * sizeof(Vector));
    if ( NULL == hm->buckets){
        free(hm);
        return NULL;
    }
    for (size_t ix = 0; ix < hm->capacity ; ix++){
        if (NULL == (hm->buckets[ix] = VectorAlloc(pair_cpy, pair_cmp, pair_free))) {
            for (; ix > 0; ix--){ // free all previously allocated vectors
                VectorFree(&hm->buckets[ix-1]);
            }
            free(hm->buckets);
            free(hm);
            return NULL;
        }
    }
    hm->pair_free = pair_free;
    hm->pair_cpy = pair_cpy;
    hm->pair_cmp = pair_cmp;
    hm->hash_func = hash_func;
    hm->size = 0;
    return hm;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void HashMapFree(HashMap **p_hash_map){
    if ( NULL == p_hash_map || NULL == *p_hash_map)
    {
        return;
    }
    BucketsFree(&(*p_hash_map)->buckets, (*p_hash_map)->capacity);
    free(*p_hash_map);
    *p_hash_map = NULL;
}

/**
 * Inserts a new pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* pair,
 * NOT the pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param pair a pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int HashMapInsert(HashMap *hash_map, Pair *pair){
    if ( NULL == hash_map ||  NULL == pair){
        return 0;
    }
    if (HashMapContainsKey(hash_map, pair->key)){ // if the pair already exist
        Vector *vec = hash_map->buckets[AdditionalHash(hash_map, pair->key)];
        Pair *to_change = NULL;
        for (size_t ix = 0 ; ix < vec->size ; ix++){
            to_change = vec->data[ix];
            if (pair->key_cmp(to_change->key, pair->key)){
                break;
            }
        }
        pair->value_free(&to_change->value);
        to_change->value = pair->value_cpy(pair->value);
        return 1; // in that case we do not want to increment size. and so no need to go to the rest of the function
    }
    Pair * new_pair = PairCopy(pair);
    // note that PairCopy dynamically allocates new_pair so it will have to be freed somewhere
    if ( NULL == new_pair){return  0;}
    hash_map->size++; // increment before checking LF.
    if (HashMapGetLoadFactor(hash_map) > HASH_MAP_MAX_LOAD_FACTOR){ // if we passed max load factor we have to rehash and change everything accordingly.
        size_t old_cap = hash_map->capacity;
        hash_map->capacity *= HASH_MAP_GROWTH_FACTOR;
        Vector **old_buckets = hash_map->buckets;
        hash_map->buckets = calloc(hash_map->capacity, sizeof(Vector));
        if ( NULL == hash_map->buckets){
            BucketsFree(&old_buckets, old_cap);
            HashMapFree(&hash_map);
            return 0;
        }
        for (size_t ix = 0; ix < hash_map->capacity ; ix++){
            if( NULL == (hash_map->buckets[ix] = VectorAlloc(hash_map->pair_cpy, hash_map->pair_cmp, hash_map->pair_free))){
                BucketsFree(&old_buckets, old_cap);
                HashMapFree(&hash_map);
                return 0;
            }
        }
        UpwardsRehash(hash_map, old_buckets, old_cap);
        BucketsFree(&old_buckets, old_cap);
        hash_map->size++;// if upward hash is called we need re increment because it will be lost.
    }
    int post_hash = AdditionalHash(hash_map, new_pair->key);
    VectorPushBack(hash_map->buckets[post_hash], new_pair);
    return 1;
}

/**
 * The function checks if the given key exists in the hash map.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return 1 if the key is in the hash map, 0 otherwise.
 */
int HashMapContainsKey(HashMap *hash_map, KeyT key){
    if ( NULL == hash_map || NULL == key){
        return 0;
    }
    int post_hash_key = AdditionalHash(hash_map, key);
    Vector* curr_vec = hash_map->buckets[post_hash_key];
    Pair * curr;
    for (size_t ix = 0 ; ix < curr_vec->size ; ix++){
        curr = PairCopy(curr_vec->data[ix]);
        if (curr == NULL){return 0;}
        if (curr->key_cmp(curr->key, key)){
            free(curr);
            return 1;
        }
        free(curr);
    }
    return 0;
}

/**
 * The function checks if the given value exists in the hash map.
 * @param hash_map a hash map.
 * @param value the value to be checked.
 * @return 1 if the value is in the hash map, 0 otherwise.
 */
int HashMapContainsValue(HashMap *hash_map, ValueT value){
    // very inefficient, how can i improve?
    if (  NULL == hash_map ||  NULL == value){
        return 0;
    }
    for (size_t ix = 0 ; ix < hash_map->capacity ; ix ++){
        Vector * vec = hash_map->buckets[ix];
        for (size_t jx = 0 ; jx < vec->size ; jx ++){
            Pair * curr_pair = VectorAt(vec, jx);
            if (curr_pair->value_cmp(curr_pair->value, value)){
                return 1;
            }
        }
    }
    return 0;
}


/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise.
 */
ValueT HashMapAt(HashMap *hash_map, KeyT key){
    if (  NULL == hash_map || NULL == key){
        return NULL;
    }
    Vector * vec =  hash_map->buckets[AdditionalHash(hash_map, key)];
    for(size_t ix = 0; ix <vec->size ; ix++) {
        Pair *pair = VectorAt(vec, ix);
        if (pair->key_cmp(pair->key, key)) {
            return pair->value;
        }
    }
    return NULL;
}


/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise.
 */
int HashMapErase(HashMap *hash_map, KeyT key){
    int ret_val = 0;
    if( NULL == hash_map ||  NULL == key){return 0;}
    int post_hash = AdditionalHash(hash_map, key);
    Vector *vec = hash_map->buckets[post_hash];
    for (size_t ix = 0; ix < vec->size ; ix++){
        Pair *pair= vec->data[ix];
        if (pair->key_cmp(pair->key, key)){
            VectorErase(vec, ix);
            hash_map->size--;
            ret_val =1;
            break;
        }
    }
    if ((HashMapGetLoadFactor(hash_map) <  HASH_MAP_MIN_LOAD_FACTOR) && (hash_map->capacity >= HASH_MAP_GROWTH_FACTOR )){
        // we do not want to get a capacity that is lower then 1
        size_t new_cap = hash_map->capacity / HASH_MAP_GROWTH_FACTOR;
        for (size_t ix = 0 ; ix< new_cap; ix++) {
            // merge bucket[ix+new_cap] into bucket[ix]:
            // because: if hash(k) % old_cap is ix or ix+new_cap then hash(k) % new_cap is ix
            Vector *old_bucket = hash_map->buckets[ix+new_cap];
            Vector *new_bucket = hash_map->buckets[ix];
            for (size_t jx = 0; jx < old_bucket->size; jx++){
                VectorPushBack(new_bucket, old_bucket->data[jx]);
            }
            VectorFree(&old_bucket);
        }
        hash_map->capacity = new_cap;
        hash_map->buckets = realloc(hash_map->buckets, sizeof(Vector) * hash_map->capacity);
        if ( NULL ==  hash_map->buckets){
            // @todo free?
            return 0;
        }
    }
    return ret_val;
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double HashMapGetLoadFactor(HashMap *hash_map){
    if ( NULL == hash_map){
        return -1;
    }
    return (double) hash_map->size/ hash_map->capacity;
}

/**
 * This function deletes all the elements in the hash map.
 * @param hash_map a hash map to be cleared.
 */
void HashMapClear(HashMap *hash_map) {
    //@todo free vectors that are lost in capacity change
    HashMapClearNoCapacity(hash_map);
    hash_map->capacity = HASH_MAP_GROWTH_FACTOR;
    hash_map->buckets = realloc(hash_map->buckets, sizeof(Vector*) * hash_map->capacity);
    //end of void func' no reason to test realloc
}
