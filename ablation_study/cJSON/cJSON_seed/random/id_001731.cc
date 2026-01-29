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
//<ID> 1731
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
    cJSON *child = (cJSON *)0;
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    int arr_size = 0;
    int validation = 0;
    cJSON_bool has_items = 0;
    cJSON_bool is_child_object = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    child = cJSON_CreateObject();
    str0 = cJSON_CreateString("one");
    str1 = cJSON_CreateString("two");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "name", cJSON_CreateString("nested"));

    // step 4: Operate
    retrieved_items = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retrieved_items);
    has_items = cJSON_HasObjectItem(root, "items");
    retrieved_child = cJSON_GetObjectItem(root, "child");
    is_child_object = cJSON_IsObject(retrieved_child);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)has_items + (int)is_child_object;
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)child;
    (void)str0;
    (void)str1;
    (void)retrieved_items;
    (void)retrieved_child;
    (void)arr_size;
    (void)has_items;
    (void)is_child_object;
    // API sequence test completed successfully
    return 66;
}