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
//<ID> 693
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
    cJSON *replacement = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *arr_item0 = (cJSON *)0;
    cJSON *arr_item1 = (cJSON *)0;
    cJSON *new_first = (cJSON *)0;
    cJSON *ref_from_root = (cJSON *)0;
    double first_value = 0.0;
    double second_value = 0.0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool is_array = 0;
    cJSON_bool replaced = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    is_array = cJSON_IsArray(retrieved_arr);
    arr_item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    arr_item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    added_ref = cJSON_AddItemReferenceToObject(root, "second_ref", arr_item1);
    replacement = cJSON_CreateNumber(99.0);
    replaced = cJSON_ReplaceItemViaPointer(retrieved_arr, arr_item0, replacement);
    new_first = cJSON_GetArrayItem(retrieved_arr, 0);
    ref_from_root = cJSON_GetObjectItem(root, "second_ref");
    first_value = cJSON_GetNumberValue(new_first);
    second_value = cJSON_GetNumberValue(ref_from_root);

    // step 5: Validate
    (void)added_arr;
    (void)added_ref;
    (void)is_array;
    (void)replaced;
    (void)first_value;
    (void)second_value;
    (void)new_first;
    (void)ref_from_root;
    (void)arr_item0;
    (void)arr_item1;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}