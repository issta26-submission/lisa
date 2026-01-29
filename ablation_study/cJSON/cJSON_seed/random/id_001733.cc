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
//<ID> 1733
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
    cJSON *elem_str = (cJSON *)0;
    cJSON *elem_num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_label = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool has_items = 0;
    cJSON_bool is_root_obj = 0;
    cJSON_bool is_label_obj = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem_str = cJSON_CreateString("one");
    elem_num = cJSON_CreateNumber(2.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, elem_str);
    cJSON_AddItemToArray(arr, elem_num);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("numbers"));

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    has_items = cJSON_HasObjectItem(root, "items");
    retrieved_label = cJSON_GetObjectItem(root, "label");
    is_root_obj = cJSON_IsObject(root);
    is_label_obj = cJSON_IsObject(retrieved_label);

    // step 5: Validate
    validation = (int)(has_items) + (int)(arr_size == 2) + (int)(is_root_obj) + (int)(is_label_obj == 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)elem_str;
    (void)elem_num;
    (void)retrieved_arr;
    (void)retrieved_label;
    (void)arr_size;
    (void)has_items;
    (void)is_root_obj;
    (void)is_label_obj;
    // API sequence test completed successfully
    return 66;
}