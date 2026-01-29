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
//<ID> 1535
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
    cJSON *entry_obj = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *arr_str1 = (cJSON *)0;
    cJSON *arr_str2 = (cJSON *)0;
    cJSON *retrieved_entry = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    char *name_str = (char *)0;
    cJSON_bool added_arr1 = 0;
    cJSON_bool added_arr2 = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_entry_to_root = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    entry_obj = cJSON_CreateObject();
    name_item = cJSON_CreateString("Alice");
    arr_str1 = cJSON_CreateString("Bob");
    arr_str2 = cJSON_CreateString("Carol");

    // step 3: Configure
    added_arr1 = cJSON_AddItemToArray(arr, arr_str1);
    added_arr2 = cJSON_AddItemToArray(arr, arr_str2);
    added_arr_to_root = cJSON_AddItemToObject(root, "names", arr);
    cJSON_AddItemToObject(entry_obj, "name", name_item);
    added_entry_to_root = cJSON_AddItemToObject(root, "entry", entry_obj);

    // step 4: Operate
    retrieved_array = cJSON_GetObjectItem(root, "names");
    retrieved_entry = cJSON_GetObjectItem(root, "entry");
    retrieved_name = cJSON_GetObjectItem(retrieved_entry, "name");
    name_str = cJSON_GetStringValue(retrieved_name);

    // step 5: Validate
    validation_score = (int)added_arr1 + (int)added_arr2 + (int)added_arr_to_root + (int)added_entry_to_root + (int)(retrieved_array != (cJSON *)0) + (int)(retrieved_entry != (cJSON *)0) + (int)(name_str != (char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_name;
    (void)retrieved_array;
    (void)name_str;
    // API sequence test completed successfully
    return 66;
}