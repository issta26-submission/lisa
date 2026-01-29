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
//<ID> 1736
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *items_arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_note = (cJSON *)0;
    cJSON_bool has_items = 0;
    cJSON_bool is_obj = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    items_arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(1.0);
    str_item = cJSON_CreateString("two");

    // step 3: Configure
    cJSON_AddItemToArray(items_arr, num_item);
    cJSON_AddItemToArray(items_arr, str_item);
    cJSON_AddItemToObject(root, "numbers", items_arr);
    cJSON_AddItemToObject(root, "note", cJSON_CreateString("example"));

    // step 4: Operate
    has_items = cJSON_HasObjectItem(root, "numbers");
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    is_obj = cJSON_IsObject(root);
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_note = cJSON_GetObjectItem(root, "note");

    // step 5: Validate
    validation = (int)has_items + (int)is_obj + (int)(arr_size == 2) + (int)(retrieved_note != (cJSON *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)items_arr;
    (void)num_item;
    (void)str_item;
    (void)retrieved_arr;
    (void)retrieved_note;
    (void)has_items;
    (void)is_obj;
    (void)arr_size;
    // API sequence test completed successfully
    return 66;
}