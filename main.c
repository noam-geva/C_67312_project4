#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashMap.h"
#include "PairCharInt.h"
#include "Hash.h"

enum errors{SUCCESS, TEST1FAIL, TEST2FAIL, TEST3FAIL, TEST4FAIL, TEST5FAIL,
    TEST6FAIL};
#define PAIR_FUNCS CharKeyCpy, IntValueCpy, CharKeyCmp, IntValueCmp, \
CharKeyFree, IntValueFree
#define FAIL 0


int Test1();
int Test2();
int Test3();
int Test4();
int Test5();
int Test6();
int main()
{
    printf("TEST1: HashMapAlloc + HashMapFree\n");
    int result_test1 = Test1();
    if(result_test1 != 0){
        fprintf(stderr, "TEST 1 FAILED\n");
        return 1;
    }
    printf("TEST 1 PASSED!\n\n");

    printf("TEST2: HashMapInsert\n");
    int result_test2 = Test2();
    if(result_test2 != 0){
        fprintf(stderr, "TEST 2 FAILED\n");
        return 2;
    }
    printf("TEST 2 PASSED!\n\n");

    printf("TEST3: HashMapAt + HashMapContainsKey + HashMapContainsValue\n");
    int result_test3 = Test3();
    if(result_test3 != 0){
        fprintf(stderr, "TEST 3 FAILED\n");
        return 3;
    }
    printf("TEST 3 PASSED!\n\n");

    printf("TEST 4: HashMapErase\n");
    int result_test4 = Test4();
    if(result_test4 != 0){
        fprintf(stderr, "TEST 4 FAILED\n");
        return 4;
    }
    printf("TEST 4 PASSED!\n\n");

    printf("TEST 5: HashMapClear\n");
    int result_test5 = Test5();
    if(result_test5 != 0){
        fprintf(stderr, "TEST 5 FAILED\n");
        return 5;
    }
    printf("TEST 5 PASSED!\n\n");

    printf("TEST 6: ALL TOGETHER\n");
    int result_test6 = Test6();
    if(result_test6 != 0){
        fprintf(stderr, "TEST 6 FAILED\n");
        return 6;
    }
    printf("TEST 6 PASSED!\n\n");

    printf("ALL TESTS PASSED :)\n");
    return 0;
}

int Test6() {
    HashMap *h_map = HashMapAlloc(HashChar, PairCharIntCpy, PairCharIntCmp,
                                  PairCharIntFree);
    if(!h_map){
        fprintf(stderr, "TEST 6: Failed to allocate hash map\n");
        return TEST6FAIL;
    }

    char char_arr[] = "abcdefghijklmno";
    void* keys[15];
    int int_arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    void* values[15];
    for(int i = 0; i < 15; i++){
        keys[i] = &char_arr[i];
        values[i] = &int_arr[i];
    }

    Pair* pair_arr[15];
    for(int i = 0; i < 15; i++){
        pair_arr[i] = PairAlloc(keys[i], values[i], PAIR_FUNCS);
        if(!pair_arr[i]){
            for(int j = 0; j < i ; j++)
            {
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 6: Failed to allocate pair%d\n", i);
            return TEST6FAIL;
        }
    }

    for(int i = 0; i < 15; i++){
        int insertion_result = HashMapInsert(h_map, pair_arr[i]);
        if(insertion_result == FAIL){
            for(int j = 0; j < 15; j++){
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 6: Failed to insert pair%d\n", i);
            return TEST6FAIL;
        }
    }
    //so far inserted 15 pairs to map

    int fail_flag = 0;
    for(int i = 0; i < 15; i++){
        if(HashMapContainsKey(h_map, keys[i]) == 1){
            int contains_val = HashMapContainsValue(h_map, HashMapAt(h_map, keys[i]));
            if(contains_val != 1){
                fail_flag = 1;
                break;
            }
        }else{
            fail_flag = 1;
            break;
        }
    }
    if (fail_flag == 1 || h_map->capacity != 32){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 6: could not find key/value that suppose to be in map"
                        " or map size incorect\n");
        return TEST6FAIL;
    }

    //erasing pairs pair_arr[0] - pair_arr[7]
    for(int i = 0; i < 8; i++){
        int erase_success = HashMapErase(h_map, keys[i]);
        if(erase_success != 1){
            fail_flag = 1;
            break;
        }
    }
    if (fail_flag == 1 || h_map->capacity != 16){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 6: could not erase some pairs that in the map or "
                        "map size incorrect\n");
        return TEST6FAIL;
    }

    //inserting pairs pair_arr[0] - pair_arr[7]
    for(int i = 0; i < 8; i++){
        int insert_success = HashMapInsert(h_map, pair_arr[i]);
        if(insert_success != 1){
            fail_flag = 1;
            break;
        }
    }
    if (fail_flag == 1 || h_map->capacity != 32){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 6: could not insert some pairs to the map or map "
                        "capacity incorrect\n");
        return TEST6FAIL;
    }

    //creating new pairs with same keys and different vals
    int other_int_arr[8] = {10,20,30,40,50,60,70,80};
    void* other_vals[8];
    for(int i = 0; i < 8; i++){
        other_vals[i] = &other_int_arr[i];
    }

    Pair* other_pair_arr[8];
    for(int i = 0; i < 8; i++) {
        other_pair_arr[i] = PairAlloc(keys[i], other_vals[i], PAIR_FUNCS);
        if (!other_pair_arr[i]) {
            for (int j = 0; j < i; j++) {
                PairCharIntFree((void *) &other_pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 6: Failed to allocate pair%d\n", i);
            fail_flag = 1;
            break;
        }
    }
    if(fail_flag == 1){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        HashMapFree(&h_map);
        return TEST6FAIL;
    }

    // inserting pair with new vals
    for(int i = 0; i < 8; i++) {
        int insertion_result = HashMapInsert(h_map, other_pair_arr[i]);
        if (insertion_result == FAIL) {
            fail_flag = 1;
            fprintf(stderr, "TEST 6: Failed to insert pair%d\n", i);
            break;
        }
    }
    if (fail_flag == 1){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        for(int i = 0; i < 8; i++){
            PairCharIntFree((void*)&other_pair_arr[i]);
        }
        HashMapFree(&h_map);
        return TEST6FAIL;
    }

    //checks if new values in map and lod are not
    for(int i = 0; i <8; i++){
        if(HashMapContainsValue(h_map, values[i]) == 1){
            fail_flag = 1;
            break;
        }
        if(HashMapContainsValue(h_map, other_vals[i]) != 1){
            fail_flag = 1;
            break;
        }
    }

    if (fail_flag == 1){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        for(int i = 0; i < 8; i++){
            PairCharIntFree((void*)&other_pair_arr[i]);
        }
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 6: incorrect values in map or correct values not "
                        "in map\n");
        return TEST6FAIL;
    }

    //clearing map
    HashMapClear(h_map);
    if(h_map->capacity != 2 || h_map->size != 0){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        for(int i = 0; i < 8; i++){
            PairCharIntFree((void*)&other_pair_arr[i]);
        }
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 6: incorrect capacity or size after clear\n");
        return TEST6FAIL;
    }

    //inserting original 15 pairs again
    for(int i = 0; i < 15; i++){
        int insertion_result = HashMapInsert(h_map, pair_arr[i]);
        if(insertion_result == FAIL){
            for(int j = 0; j < 15; j++){
                PairCharIntFree((void*)&pair_arr[j]);
            }
            for(int k = 0; k < 8; k++){
                PairCharIntFree((void*)&other_pair_arr[k]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 6: Failed to insert pair%d\n", i);
            return TEST6FAIL;
        }
    }

    if(h_map->size != 15 || h_map->capacity != 32){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        for(int k = 0; k < 8; k++){
            PairCharIntFree((void*)&other_pair_arr[k]);
        }
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 6: incorrect size or capacity of map\n");
        return TEST6FAIL;
    }

    for(int j = 0; j < 15; j++){
        PairCharIntFree((void*)&pair_arr[j]);
    }
    for(int k = 0; k < 8; k++){
        PairCharIntFree((void*)&other_pair_arr[k]);
    }
    HashMapFree(&h_map);
    return SUCCESS;
}

int Test5() {
    HashMap *h_map = HashMapAlloc(HashChar, PairCharIntCpy, PairCharIntCmp,
                                  PairCharIntFree);
    if(!h_map){
        fprintf(stderr, "TEST 5: Failed to allocate hash map\n");
        return TEST5FAIL;
    }

    char char_arr[] = "abcdefghijklmno";
    void* keys[15];
    int int_arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    void* values[15];
    for(int i = 0; i < 15; i++){
        keys[i] = &char_arr[i];
        values[i] = &int_arr[i];
    }

    Pair* pair_arr[15];
    for(int i = 0; i < 15; i++){
        pair_arr[i] = PairAlloc(keys[i], values[i], PAIR_FUNCS);
        if(!pair_arr[i]){
            for(int j = 0; j < i ; j++)
            {
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 5: Failed to allocate pair%d\n", i);
            return TEST5FAIL;
        }
    }

    for(int i = 0; i < 15; i++){
        int insertion_result = HashMapInsert(h_map, pair_arr[i]);
        if(insertion_result == FAIL){
            for(int j = 0; j < 15; j++){
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 5: Failed to insert pair%d\n", i);
            return TEST5FAIL;
        }
    }

    HashMapClear(h_map);
    for(int i = 0; i < 15; i++){
        if(HashMapContainsKey(h_map, keys[i]) || HashMapContainsValue(h_map,
                                                                      values[i])){
            for(int j = 0; j < 15; j++){
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 5: found key/value of pair #%d\n", i+1);
            return TEST5FAIL;
        }
    }

    if(h_map->size != 0 || h_map->capacity != 2){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 5: size/ capacity after erase incorrect\n");
        return TEST5FAIL;
    }

    for(int j = 0; j < 15; j++){
        PairCharIntFree((void*)&pair_arr[j]);
    }
    HashMapFree(&h_map);
    return SUCCESS;

}

int Test4() {
    HashMap *h_map = HashMapAlloc(HashChar, PairCharIntCpy, PairCharIntCmp,
                                  PairCharIntFree);
    if(!h_map){
        fprintf(stderr, "TEST 4: Failed to allocate hash map\n");
        return TEST4FAIL;
    }

    char char_arr[] = "abcdefghijklmno";
    void* keys[15];
    int int_arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    void* values[15];
    for(int i = 0; i < 15; i++){
        keys[i] = &char_arr[i];
        values[i] = &int_arr[i];
    }

    Pair* pair_arr[15];
    for(int i = 0; i < 15; i++){
        pair_arr[i] = PairAlloc(keys[i], values[i], PAIR_FUNCS);
        if(!pair_arr[i]){
            for(int j = 0; j < i ; j++)
            {
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 4: Failed to allocate pair%d\n", i);
            return TEST4FAIL;
        }
    }

    for(int i = 0; i < 15; i++){
        int insertion_result = HashMapInsert(h_map, pair_arr[i]);
        if(insertion_result == FAIL){
            for(int j = 0; j < 15; j++){
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 4: Failed to insert pair%d\n", i);
            return TEST4FAIL;
        }
    }

    //done initializing map- test stars here
    Pair* to_remove1 = pair_arr[0];
    int fail_flag = 0;
    int remove1_result = HashMapErase(h_map, to_remove1->key);
    if(remove1_result == FAIL){
        fprintf(stderr, "TEST 4: failed to erase pair1: <'a',1>\n");
        fail_flag = 1;
    }
    ValueT pair1_value = HashMapAt(h_map, to_remove1->key);
    int contains_key_a = HashMapContainsKey(h_map, to_remove1->key);
    int contains_val_1 = HashMapContainsValue(h_map, to_remove1->value);
    if(pair1_value != NULL){
        fprintf(stderr, "TEST 4: The value of removed pair was found\n");
        fail_flag = 1;
    }
    if(contains_key_a != FAIL){
        fprintf(stderr, "TEST 4: found a key that should not be in map\n");
        fail_flag = 1;
    }
    if(contains_val_1 != FAIL){
        fprintf(stderr, "TEST 4: found a value that should not be in map\n");
        fail_flag = 1;
    }
    if(h_map->size != 14){
        fprintf(stderr, "TEST 4: hash map size incorrect after erase\n");
        fail_flag = 1;
    }
    if(fail_flag){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        HashMapFree(&h_map);
        return TEST4FAIL;
    }

    //removing many pairs to resize buckets
    for(int i = 1; i <= 8; i++){
        Pair* pair_to_remove = pair_arr[i];
        int remove_result = HashMapErase(h_map, pair_to_remove->key);
        if(remove_result == FAIL){
            fprintf(stderr, "TEST 4: failed to remove pair in pair_arr[%d]\n", i);
            fail_flag = 1;
            break;
        }
        if(HashMapContainsKey(h_map, pair_to_remove->key) || HashMapContainsValue
                (h_map, pair_to_remove->value)){
            fprintf(stderr, "TEST 4: removed pair was found by key/value\n");
            fail_flag = 1;
            break;
        }
        size_t expected_size = 14 - i;
        if(h_map->size != expected_size){
            fprintf(stderr, "TEST 4: hash map size is incorrect\n");
            fail_flag = 1;
            break;
        }
    }
    if(fail_flag){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        HashMapFree(&h_map);
        return TEST4FAIL;
    }

    //removing pair not in map
    int key_not_in_map = 'z';
    void* p_val_not_in_map = &key_not_in_map;
    int result_not_in_map = HashMapErase(h_map, p_val_not_in_map);
    if(result_not_in_map != FAIL){
        fprintf(stderr, "TEST 4: key that not in map was removed or success "\
                    "return value was returned\n");
        fail_flag = 1;
    }
    if(h_map->size != 6){
        fprintf(stderr, "TEST 4: hash map size is incorrect\n");
        fail_flag = 1;
    }
    if(fail_flag){
        for(int j = 0; j < 15; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        HashMapFree(&h_map);
        return TEST4FAIL;
    }

    for(int j = 0; j < 15; j++){
        PairCharIntFree((void*)&pair_arr[j]);
    }
    HashMapFree(&h_map);
    return SUCCESS;
}

int Test3() {
    // initializing hash map
    HashMap *h_map = HashMapAlloc(HashChar, PairCharIntCpy, PairCharIntCmp,
                                  PairCharIntFree);
    if(!h_map){
        fprintf(stderr, "TEST 3: Failed to allocate hash map\n");
        return TEST3FAIL;
    }

    char char_arr[] = "abcdefghijklmno";
    void* keys[15];
    int int_arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    void* values[15];
    for(int i = 0; i < 15; i++){
        keys[i] = &char_arr[i];
        values[i] = &int_arr[i];
    }

    Pair* pair_arr[15];
    for(int i = 0; i < 15; i++){
        pair_arr[i] = PairAlloc(keys[i], values[i], PAIR_FUNCS);
        if(!pair_arr[i]){
            for(int j = 0; j < i ; j++)
            {
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 3: Failed to allocate pair%d\n", i);
            return TEST3FAIL;
        }
    }

    for(int i = 0; i < 15; i++){
        int insertion_result = HashMapInsert(h_map, pair_arr[i]);
        if(insertion_result == FAIL){
            for(int j = 0; j < 15; j++){
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 3: Failed to insert pair%d\n", i);
            return TEST3FAIL;
        }
    }
    //done initializing hash map- test starts here
    for(int i = 0; i < 15; i++){
        int contains_key = HashMapContainsKey(h_map, keys[i]);
        if(contains_key == 0){
            for(int j = 0; j < 15; j++){
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 3: cannot find key <%c>\n", i + 97);
            return TEST3FAIL;
        }
    }

    for(int i = 0; i < 15; i++){
        int contains_val = HashMapContainsValue(h_map, values[i]);
        if(contains_val == 0){
            for(int j = 0; j < 15; j++){
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 3: cannot find value value <%d>\n", i + 1);
            return TEST3FAIL;
        }
    }

    for(int i = 0; i < 15; i++){
        void* value_of_key = HashMapAt(h_map, keys[i]);
        if(*((int*)value_of_key) != i+1){
            for(int j = 0; j < 15; j++){
                PairCharIntFree((void*)&pair_arr[j]);
            }
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 3: value of pair %d incorrect\n", i + 1);
            return TEST3FAIL;
        }
    }

    char key_not_in_map = 'z';
    int val_not_in_map = 800;
    void* p_key_not_in = &key_not_in_map;
    void* p_val_not_in = &val_not_in_map;
    int fail_flag = 0;
    if(HashMapContainsKey(h_map, p_key_not_in)){
        fprintf(stderr, "TEST 3: found a key not in map\n");
        fail_flag = 1;
    }
    if(HashMapContainsValue(h_map,p_val_not_in)){
        fprintf(stderr, "TEST 3: found a value not in map\n");
        fail_flag = 1;
    }
    if(HashMapAt(h_map, p_key_not_in) != NULL){
        fprintf(stderr, "TEST 3: returned a value not associated with key\n");
        fail_flag = 1;
    }

    for(int j = 0; j < 15; j++){
        PairCharIntFree((void*)&pair_arr[j]);
    }
    HashMapFree(&h_map);
    if(fail_flag){
        return TEST3FAIL;
    }
    return SUCCESS;
}

int Test2() {
    HashMap *h_map = HashMapAlloc(HashChar, PairCharIntCpy, PairCharIntCmp,
                                  PairCharIntFree);
    if(!h_map){
        fprintf(stderr, "TEST 2: Failed to allocate hash map\n");
        return TEST2FAIL;
    }

    //inserting one pair
    char pair1_key = 'a';
    int pair1_val = 1;
    void *p1_key_p = &pair1_key, *p1_val_p = &pair1_val;
    Pair* pair1 = PairAlloc(p1_key_p, p1_val_p, PAIR_FUNCS);
    if(!pair1){
        fprintf(stderr, "TEST 2: Failed to allocate pair1\n");
        HashMapFree(&h_map);
        return TEST2FAIL;
    }
    int pair1_insertion = HashMapInsert(h_map, pair1);
    if(pair1_insertion == FAIL){
        PairCharIntFree((void*)&pair1);
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 2: Failed to insert pair1\n");
        return TEST2FAIL;
    }
    // inserting many pairs to cause resizing buckets
    char char_arr[] = "bcdefghijklmno";
    void* keys[14];
    int int_arr[] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    void* values[14];
    for(int i = 0; i < 14; i++){
        keys[i] = &char_arr[i];
        values[i] = &int_arr[i];
    }

    Pair* pair_arr[14];
    for(int i = 0; i < 14; i++){
        pair_arr[i] = PairAlloc(keys[i], values[i], PAIR_FUNCS);
        if(!pair_arr[i]){
            for(int j = 0; j < i ; j++)
            {
                PairCharIntFree((void*)&pair_arr[j]);
            }
            PairCharIntFree((void*)&pair1);
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 2: Failed to allocate pair%d\n", i);
            return TEST2FAIL;
        }
    }

    for(int i = 0; i < 14; i++){
        int insertion_result = HashMapInsert(h_map, pair_arr[i]);
        if(insertion_result == FAIL){
            for(int j = 0; j < 14; j++){
                PairCharIntFree((void*)&pair_arr[j]);
            }
            PairCharIntFree((void*)&pair1);
            HashMapFree(&h_map);
            fprintf(stderr, "TEST 2: Failed to insert pair%d\n", i);
            return TEST2FAIL;
        }
    }

    if(h_map->capacity != HASH_MAP_GROWTH_FACTOR * HASH_MAP_INITIAL_CAP ||
       h_map->size != 15){
        for(int j = 0; j < 14; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        PairCharIntFree((void*)&pair1);
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 2: HashMap capacity or size incorrect\n");
        return TEST2FAIL;
    }

    //inserting value with key already entered
    int new_val = 100;
    void* p_new_value = &new_val;
    Pair* same_key_pair = PairAlloc(keys[2], p_new_value, PAIR_FUNCS);
    if(!same_key_pair){
        for(int j = 0; j < 14; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        PairCharIntFree((void*)&pair1);
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 2: failed to allocate pair with same key\n");
        return TEST2FAIL;
    }
    int duplicate_insertion_result = HashMapInsert(h_map, same_key_pair);
    if(duplicate_insertion_result == FAIL){
        for(int j = 0; j < 14; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        PairCharIntFree((void*)&pair1);
        PairCharIntFree((void*)&same_key_pair);
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 2: failed to override old pair\n");
        return TEST2FAIL;
    }

    int fail_flag = 0;
    size_t index = (h_map->hash_func(same_key_pair->key)) & (h_map->capacity - 1);
    Vector *contains_key = h_map->buckets[index];
    if(contains_key){
        for(size_t i = 0; i < contains_key->size; i++){
            Pair *cur_pair = (Pair*)contains_key->data[i];
            if(cur_pair->key_cmp(cur_pair->key, same_key_pair->key)){
                if(cur_pair->value_cmp(cur_pair->value, same_key_pair->value)){
                    break;
                }else{
                    fail_flag = 1;
                }
            }
        }
    }else{
        fail_flag = 1;
    }

    if(fail_flag || h_map->size != 15){
        for(int j = 0; j < 14; j++){
            PairCharIntFree((void*)&pair_arr[j]);
        }
        PairCharIntFree((void*)&pair1);
        PairCharIntFree((void*)&same_key_pair);
        HashMapFree(&h_map);
        fprintf(stderr, "TEST 2: failed to override old pair or incorrect size "
                        "of hash map\n");
        return TEST2FAIL;
    }



    for(int i = 0; i < 14; i++){
        PairCharIntFree((void*)&pair_arr[i]);
    }
    PairCharIntFree((void*)&pair1);
    PairCharIntFree((void*)&same_key_pair);
    HashMapFree(&h_map);
    return SUCCESS;
}

int Test1() {
    HashMap *h_map = HashMapAlloc(HashChar, PairCharIntCpy, PairCharIntCmp,
                                  PairCharIntFree);
    if(!h_map){
        fprintf(stderr, "TEST 1: Failed to allocate hash map\n");
        return TEST1FAIL;
    }
    if(h_map->size != 0 || h_map->capacity != HASH_MAP_INITIAL_CAP){
        fprintf(stderr, "TEST 1: Initial values of allocations incorrect\n");
        return TEST1FAIL;
    }
    HashMapFree(&h_map);
    return SUCCESS;
}


/**
 * =====================================================================================================================
 */

/**

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
    assert(vector->capacity == 2);

    VectorFree(&vector);

    vector = VectorAlloc(ElemCpy, ElemCmp, ElemFree);
    for (int k_i = 0; k_i < 1; ++k_i) {
        VectorPushBack(vector, &k_i);
    }

    VectorClear(vector);
    assert(vector->size == 0);
    assert(vector->capacity == 2);

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




 */


/**
 * =====================================================================================================================
 */




/**
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vector.h"

enum errors{SUCCESS, TEST1FAIL, TEST2FAIL, TEST3FAIL, TEST4FAIL, TEST5FAIL,
    TEST6FAIL};
int IntCompare(const void* val1, const void* val2);
void* IntCopy(const void* val);
void IntFree(void **to_free);

int Test1();
int Test2();
int Test3();

int Test4();
int Test5();
int Test6();
int main()
{
    printf("TEST 1: VectorAlloc\n");
    int result_test1 = Test1();
    if(result_test1 != 0){
        fprintf(stderr, "Test 1 FAILED!\n\n");
        exit(1);
    }
    printf("TEST 1 PASSED!\n\n");

    printf("TEST 2: VectorAt\n");
    int result_test2 = Test2();
    if(result_test2 != 0){
        fprintf(stderr, "Test 2 FAILED!\n\n");
        exit(2);
    }
    printf("TEST 2 PASSED!\n\n");

    printf("TEST 3: VectorFind\n");
    int result_test3 = Test3();
    if(result_test3 != 0){
        fprintf(stderr, "Test 3 FAILED!\n\n");
        exit(3);
    }
    printf("TEST 3 PASSED!\n\n");

    printf("TEST 4: VectorPushBack\n");
    int result_test4 = Test4();
    if(result_test4 != 0){
        fprintf(stderr, "Test 4 FAILED!\n\n");
        exit(4);
    }
    printf("TEST 4 PASSED!\n\n");

    printf("TEST 5: VectorErase\n");
    int result_test5 = Test5();
    if(result_test5 != 0){
        fprintf(stderr, "Test 5 FAILED!\n\n");
        exit(5);
    }
    printf("TEST 5 PASSED!\n\n");

    printf("TEST 6: VectorClear\n");
    printf("NOTICE: I couldn't really check if your vec has been cleared for\n "\
         "values so I am just checking if size and capacity set correctly.\n "\
         "MAKE SURE TO RESIZE YOUR DATA ARRAY!\n\n");
    int result_test6 = Test6();
    if(result_test6 != 0){
        fprintf(stderr, "Test 6 FAILED!\n\n");
        exit(6);
    }
    printf("TEST 6 PASSED!\n\n");
    printf("ALL TESTS PASSED :)\n");
}

int Test6() {
    Vector *vec = VectorAlloc(IntCopy, IntCompare, IntFree);
    if(!vec){
        fprintf(stderr, "TEST 6: int_vec allocation failed\n");
        return TEST6FAIL;
    }
    int num_arr[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    void* *num_p_array = malloc(20 * sizeof(void*));
    if(!num_p_array){
        VectorFree(&vec);
        fprintf(stderr, "TEST 6: pointers array allocation failed\n");
        return TEST6FAIL;
    }
    for(int i = 0; i < 20; i++){
        num_p_array[i] = &num_arr[i];
    }

    for(int i = 0; i < 20; i++){
        int result = VectorPushBack(vec, num_p_array[i]);
        if(result == 0)
        {
            VectorFree(&vec);
            free(num_p_array);
            fprintf(stderr, "TEST 6: inserting elem #%d to vector failed\n", i);
            return TEST6FAIL;
        }
    }

    VectorClear(vec);
    if(vec->size != 0 || vec->capacity != 2){
        VectorFree(&vec);
        free(num_p_array);
        fprintf(stderr, "TEST 6: size or capacity haven't been resized\n");
        return TEST6FAIL;
    }
    VectorFree(&vec);
    free(num_p_array);
    return SUCCESS;
}

int Test5() {
    Vector *vec = VectorAlloc(IntCopy, IntCompare, IntFree);
    if(!vec){
        fprintf(stderr, "TEST 5: int_vec allocation failed\n");
        return TEST5FAIL;
    }
    int num_arr[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    void* *num_p_array = malloc(20 * sizeof(void*));
    if(!num_p_array){
        VectorFree(&vec);
        fprintf(stderr, "TEST 5: pointers array allocation failed\n");
        return TEST5FAIL;
    }
    for(int i = 0; i < 20; i++){
        num_p_array[i] = &num_arr[i];
    }

    for(int i = 0; i < 20; i++){
        int result = VectorPushBack(vec, num_p_array[i]);
        if(result == 0)
        {
            VectorFree(&vec);
            free(num_p_array);
            fprintf(stderr, "TEST 5: inserting elem #%d to vector failed\n", i);
            return TEST5FAIL;
        }
    }

    int erase_invalid = VectorErase(vec,20);
    if(erase_invalid != 0 || vec->size != 20){
        fprintf(stderr, "TEST 5: erasing invalid index failed\n");
        free(num_p_array);
        VectorFree(&vec);
        return TEST5FAIL;
    }

    int erase_ind_0 = VectorErase(vec, 0);
    int erase_ind_10 = VectorErase(vec,10);
    int erase_ind_17 = VectorErase(vec,17);
    if(vec->size != 17 || erase_ind_0 != 1 || erase_ind_17 != 1 || erase_ind_10
                                                                   != 1){
        fprintf(stderr, "TEST 5: erasing indexes 0, 17, 10 failed\n");
        free(num_p_array);
        VectorFree(&vec);
        return TEST5FAIL;
    }
    if(*((int*)vec->data[0]) != 2 || *((int*)vec->data[10]) != 13 || *((int*)
            vec->data[16]) != 19){
        fprintf(stderr, "TEST 5: data array values didnt move back after "\
                    "erasing\n");
        free(num_p_array);
        VectorFree(&vec);
        return TEST5FAIL;
    }

    for(int i = 0; i < 10; i++){
        int erase = VectorErase(vec, 0);
        if(erase != 1){
            fprintf(stderr, "TEST 5: erasing indexes failed\n");
            free(num_p_array);
            VectorFree(&vec);
            return TEST5FAIL;
        }
    }

    if(vec->size != 7 || vec->capacity != 16){
        fprintf(stderr, "TEST 5: vector have not been resized correctly after "
                        "deleting elements\n");
        free(num_p_array);
        VectorFree(&vec);
        return TEST5FAIL;
    }
    free(num_p_array);
    VectorFree(&vec);
    return SUCCESS;
}

int Test4() {
    Vector *vec = VectorAlloc(IntCopy, IntCompare, IntFree);
    if(!vec){
        fprintf(stderr, "TEST 4: int_vec allocation failed\n");
        return TEST4FAIL;
    }
    int num_arr[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    void* *num_p_array = malloc(20 * sizeof(void*));
    if(!num_p_array){
        VectorFree(&vec);
        fprintf(stderr, "TEST 4: pointers array allocation failed\n");
        return TEST4FAIL;
    }
    for(int i = 0; i < 20; i++){
        num_p_array[i] = &num_arr[i];
    }

    for(int i = 0; i < 20; i++){
        int result = VectorPushBack(vec, num_p_array[i]);
        if(result == 0)
        {
            VectorFree(&vec);
            free(num_p_array);
            fprintf(stderr, "TEST 4: inserting elem #%d to vector failed\n", i);
            return TEST4FAIL;
        }
    }

    //check if all inserted correctly
    for(int i = 0; i < 20; i++)
    {
        void* elem = VectorAt(vec, i);
        if(*((int*)elem) != i + 1)
        {
            VectorFree(&vec);
            free(num_p_array);
            fprintf(stderr, "TEST 4: element returned from vector in iteration "\
                      "#%d incorrect (before release)\n", i);
            return TEST4FAIL;
        }
    }

    free(num_p_array);

    for(int i = 0; i < 20; i++)
    {
        void* elem = VectorAt(vec, i);
        if(*((int*)elem) != i + 1)
        {
            VectorFree(&vec);
            fprintf(stderr, "TEST 4: element returned from vector in iteration "\
                      "#%d incorrect (after release)\n", i);
            return TEST4FAIL;
        }
    }
    VectorFree(&vec);
    return SUCCESS;
}

int Test3() {
    Vector *int_vec = VectorAlloc(IntCopy, IntCompare, IntFree);
    if(!int_vec){
        fprintf(stderr, "TEST 3: int_vec allocation failed\n");
        return TEST3FAIL;
    }
    //create values and insert manually
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    void* p_num1 = &num1;
    void* p_num2 = &num2;
    void* p_num3 = &num3;
    int_vec->data[0] = p_num1;
    int_vec->data[1] = p_num2;
    int_vec->data[2] = p_num3;
    int_vec->size +=3;
    int index1 = VectorFind(int_vec, p_num1);
    int index2 = VectorFind(int_vec, p_num2);
    int index3 = VectorFind(int_vec, p_num3);
    if(index1 != 0 || index2 != 1 || index3 != 2){
        //freeing vector manually because elements were not allocated!
        free(int_vec->data);
        free(int_vec);
        fprintf(stderr, "TEST 3: returned wrong index\n");
        return TEST3FAIL;
    }

    int num4 = 4;
    void *p_num4 = &num4;
    int index_not_found1 = VectorFind(int_vec, p_num4);
    int index_not_found2 = VectorFind(NULL, p_num4);
    int index_not_found3 = VectorFind(int_vec, NULL);
    if(index_not_found1 != -1 || index_not_found2 != -1 || index_not_found3 !=-1){
        //freeing vector manually because elements were not allocated!
        free(int_vec->data);
        free(int_vec);
        fprintf(stderr, "TEST 3: returned wrong value when should return -1\n");
        return TEST3FAIL;
    }
    //freeing vector manually because elements were not allocated!
    free(int_vec->data);
    free(int_vec);
    return SUCCESS;
}

int Test2() {
    Vector *int_vec = VectorAlloc(IntCopy, IntCompare, IntFree);
    if(!int_vec){
        fprintf(stderr, "TEST 2: int_vec allocation failed\n");
        return TEST2FAIL;
    }
    //create values and insert manually
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    void* p_num1 = &num1;
    void* p_num2 = &num2;
    void* p_num3 = &num3;
    int_vec->data[0] = p_num1;
    int_vec->data[1] = p_num2;
    int_vec->data[2] = p_num3;
    int_vec->size +=3;
    void* ret_num1 = VectorAt(int_vec, 0);
    void* ret_num2 = VectorAt(int_vec, 1);
    void* ret_num3 = VectorAt(int_vec, 2);
    void* ret_null1 = VectorAt(int_vec, -3);
    void* ret_null2 = VectorAt(int_vec, 3);

    if(ret_null1 != NULL || ret_null2 != NULL){
        //freeing vector manually because elements were not allocated!
        free(int_vec->data);
        free(int_vec);
        fprintf(stderr, "TEST 2: ret_null1 or ret_null2 != NULL\n");
        return TEST2FAIL;
    }
    if(*(int*)ret_num1 != 1 || *(int*)ret_num2 != 2 || *(int*)ret_num3 != 3)
    {
        //freeing vector manually because elements were not allocated!
        free(int_vec->data);
        free(int_vec);
        fprintf(stderr, "TEST 2: some value didn't return as expected\n");
        return TEST2FAIL;
    }
    if(p_num1 != ret_num1 || p_num2 != ret_num2 || p_num3 != ret_num3){
        //freeing vector manually because elements were not allocated!
        free(int_vec->data);
        free(int_vec);
        fprintf(stderr, "TEST 2: pointers are not matching\n");
        return TEST2FAIL;
    }
    //freeing vector manually because elements were not allocated!
    free(int_vec->data);
    free(int_vec);
    return SUCCESS;
}

int Test1() {
    Vector *vector1 = VectorAlloc(IntCopy, IntCompare, IntFree);
    if(!vector1 || vector1->size != 0 || vector1->capacity != VECTOR_INITIAL_CAP){
        fprintf(stderr, "TEST 1: vector1 allocation failed or incorrect values\n");
        return TEST1FAIL;
    }
    VectorFree(&vector1);

    Vector *vector_missing_func = VectorAlloc(IntCopy, IntCompare, NULL);
    if(vector_missing_func){
        fprintf(stderr, "TEST 1: vector_missing_func allocation returned allocated"\
                    "vector\n");
        VectorFree(&vector_missing_func);
        return TEST1FAIL;
    }
    return SUCCESS;

}


int IntCompare(const void* val1, const void* val2){
    if(!val1 || !val2)
    {
        fprintf(stderr, "IntCompare got NULL data\n");
        exit(0);
    }
    int int1 = *((int*)val1);
    int int2 = *((int*)val2);
    return int1 == int2 ? 1 : 0;
}

void* IntCopy(const void* val){
    if(!val){
        fprintf(stderr, "IntCopy got NULL data\n");
        exit(0);
    }
    void* copy = malloc(sizeof(void*));
    if(!copy){
        fprintf(stderr, "IntCopy memory allocation failed\n");
        exit(0);
    }
    memcpy(copy, val, sizeof(int));
    return copy;
}

void IntFree(void **to_free){
    if(!to_free ||!(*to_free)){
        fprintf(stderr, "IntFree got NULL pointer\n");
        exit(0);
    }
    free(*to_free);
    *to_free = NULL;
}


 */


/**
 * =====================================================================================================================
 */

/**

#include <stdio.h>
#include "PairCharInt.h"
#include "HashMap.h"
#include "Vector.h"
#include "Pair.h"
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
    if (elem == NULL){return;}
    free(*elem);
}
int main() {
    // Insert elements to vector.
    Vector *vector = VectorAlloc(ElemCpy, ElemCmp, ElemFree);
    for (int k_i = 0; k_i < 8; ++k_i) {
        VectorPushBack(vector, &k_i);
    }
    VectorFree(&vector);
    // Create Pairs.
    Pair *pairs[8];
    for (int k_i = 0; k_i < 8; ++k_i) {
        char key = (char) (((k_i) % 75) + 48);
        int value = k_i;
        pairs[k_i] = PairAlloc(&key, &value, CharKeyCpy, IntValueCpy,
                               CharKeyCmp,
                               IntValueCmp, CharKeyFree, IntValueFree);
    }
    // Create hash-map and inserts elements into it.
    // Uses the Char-Int pairs you received.
    HashMap *hash_map = HashMapAlloc(HashChar, PairCharIntCpy,
                                     PairCharIntCmp, PairCharIntFree);
    for (int kI = 0; kI < 8; ++kI) {
        HashMapInsert(hash_map, pairs[kI]);
    }
    // Free the pairs.
    for (int k_i = 0; k_i < 8; ++k_i) {
        PairFree(&pairs[k_i]);
    }
    // Free the hash-map.
    HashMapFree(&hash_map);
    printf("passed");
    return 0;
}

 */