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
//<ID> 1730
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    int arr_size = 0;
    int validation = 0;
    cJSON_bool has_label = 0;
    cJSON_bool root_is_object = 0;
    cJSON_bool items_is_object = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    label = cJSON_CreateString("numbers");

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    add_ok = cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    retrieved_items = cJSON_GetObjectItem(root, "items");
    has_label = cJSON_HasObjectItem(root, "label");
    root_is_object = cJSON_IsObject(root);
    items_is_object = cJSON_IsObject(retrieved_items);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(retrieved_items != (cJSON *)0) + (int)(has_label != 0) + (int)(root_is_object != 0) + (int)(items_is_object == 0);
    (void)validation;
    (void)add_ok;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)label;
    (void)retrieved_items;
    (void)arr_size;
    (void)has_label;
    (void)root_is_object;
    (void)items_is_object;
    // API sequence test completed successfully
    return 66;
}