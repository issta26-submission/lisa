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
//<ID> 1540
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
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    cJSON *retrieved_raw = (cJSON *)0;
    char *printed = (char *)0;
    const char *elem_str = (const char *)0;
    cJSON_bool added_to_array = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_raw_to_obj = 0;
    cJSON_bool added_name = 0;
    cJSON_bool is_raw = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("first_element");
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");

    // step 3: Configure
    added_to_array = cJSON_AddItemToArray(arr, str_item);
    added_arr_to_obj = cJSON_AddItemToObject(root, "list", arr);
    added_name = (cJSON_AddStringToObject(root, "name", "sample") != (cJSON *)0);
    added_raw_to_obj = cJSON_AddItemToObject(root, "rawdata", raw_item);

    // step 4: Operate
    retrieved_list = cJSON_GetObjectItem(root, "list");
    retrieved_elem = cJSON_GetArrayItem(retrieved_list, 0);
    elem_str = cJSON_GetStringValue(retrieved_elem);
    retrieved_raw = cJSON_GetObjectItem(root, "rawdata");
    is_raw = cJSON_IsRaw(retrieved_raw);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_to_array
        + (int)added_arr_to_obj
        + (int)added_name
        + (int)added_raw_to_obj
        + (int)(retrieved_list != (cJSON *)0)
        + (int)(retrieved_elem != (cJSON *)0)
        + (int)(elem_str != (const char *)0)
        + (int)is_raw
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)elem_str;
    (void)retrieved_raw;
    // API sequence test completed successfully
    return 66;
}