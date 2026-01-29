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
//<ID> 1547
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
    cJSON *str_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *added_title_ptr = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *array_elem = (cJSON *)0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_raw_to_array = 0;
    cJSON_bool is_raw_flag = 0;
    const char *title_str = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("label");
    raw_item = cJSON_CreateRaw("{\"raw\":true}");

    // step 3: Configure
    added_arr_to_obj = cJSON_AddItemToObject(root, "items", arr);
    added_raw_to_array = cJSON_AddItemToArray(arr, raw_item);
    added_title_ptr = cJSON_AddStringToObject(root, "title", "example");
    cJSON_AddItemToObject(root, "meta", str_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    array_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    is_raw_flag = cJSON_IsRaw(array_elem);
    title_str = cJSON_GetStringValue(added_title_ptr);

    // step 5: Validate
    validation_score = (int)added_arr_to_obj
        + (int)added_raw_to_array
        + (int)(added_title_ptr != (cJSON *)0)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(array_elem != (cJSON *)0)
        + (int)is_raw_flag
        + (int)(title_str != (const char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)title_str;
    (void)array_elem;
    (void)retrieved_arr;
    (void)added_title_ptr;
    (void)added_arr_to_obj;
    (void)added_raw_to_array;
    (void)is_raw_flag;
    // API sequence test completed successfully
    return 66;
}