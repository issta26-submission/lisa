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
//<ID> 697
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *newnum = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *arr_item0 = (cJSON *)0;
    cJSON *ref_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool replaced = 0;
    cJSON_bool is_array = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    added_arr_to_root = cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateString("demo"));

    // step 3: Configure
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    is_array = cJSON_IsArray(retrieved_arr);
    arr_item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    added_ref = cJSON_AddItemReferenceToObject(root, "orig_ref", arr_item0);
    newnum = cJSON_CreateNumber(42.0);
    replaced = cJSON_ReplaceItemViaPointer(retrieved_arr, arr_item0, newnum);

    // step 4: Operate
    ref_item = cJSON_GetObjectItem(root, "orig_ref");
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)added_arr_to_root;
    (void)added_ref;
    (void)replaced;
    (void)is_array;
    (void)ref_item;
    (void)printed;
    (void)arr_item0;
    (void)retrieved_arr;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}