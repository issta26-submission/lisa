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
//<ID> 1549
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
    cJSON *title_ptr = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    const char *first_str = (const char *)0;
    char *printed = (char *)0;
    cJSON_bool added_str_to_arr = 0;
    cJSON_bool added_raw_to_arr = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool is_raw = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");
    raw_item = cJSON_CreateRaw("some_raw_fragment");

    // step 3: Configure
    added_str_to_arr = cJSON_AddItemToArray(arr, str_item);
    added_raw_to_arr = cJSON_AddItemToArray(arr, raw_item);
    added_arr_to_obj = cJSON_AddItemToObject(root, "list", arr);
    title_ptr = cJSON_AddStringToObject(root, "title", "mytitle");

    // step 4: Operate
    retrieved_elem = cJSON_GetArrayItem(arr, 1);
    is_raw = cJSON_IsRaw(retrieved_elem);
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_str = cJSON_GetStringValue(first_elem);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_str_to_arr
        + (int)added_raw_to_arr
        + (int)added_arr_to_obj
        + (int)is_raw
        + (int)(first_str != (const char *)0)
        + (int)(printed != (char *)0)
        + (int)(title_ptr != (cJSON *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_str;
    (void)retrieved_elem;
    (void)first_elem;
    (void)raw_item;
    (void)str_item;
    // API sequence test completed successfully
    return 66;
}