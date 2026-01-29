#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1739
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_meta = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool has_items = 0;
    cJSON_bool is_meta_obj = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("alpha");
    str1 = cJSON_CreateString("beta");
    meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "note", cJSON_CreateString("info"));

    // step 3: Configure
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    has_items = cJSON_HasObjectItem(root, "items");
    retrieved_meta = cJSON_GetObjectItem(root, "meta");
    is_meta_obj = cJSON_IsObject(retrieved_meta);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(has_items != 0) + (int)(is_meta_obj != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)str0;
    (void)str1;
    (void)meta;
    (void)retrieved_arr;
    (void)retrieved_meta;
    (void)arr_size;
    (void)has_items;
    (void)is_meta_obj;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}