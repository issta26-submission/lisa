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
//<ID> 1482
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *farr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem_str = (cJSON *)0;
    cJSON *elem_raw = (cJSON *)0;
    cJSON *str_node = (cJSON *)0;
    cJSON *raw_node = (cJSON *)0;
    char *printed = (char *)0;
    const float nums[3] = {1.1f, 2.2f, 3.3f};
    const char *str_val = (const char *)0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool added = 0;
    cJSON_bool is_raw = 0;

    // step 2: Initialize - create root object and a float array
    root = cJSON_CreateObject();
    farr = cJSON_CreateFloatArray(nums, 3);

    // step 3: Configure - add a string and a raw value into the array, attach array to root
    str_node = cJSON_CreateString("example string");
    raw_node = cJSON_CreateRaw("null");
    cJSON_AddItemToArray(farr, str_node);
    cJSON_AddItemToArray(farr, raw_node);
    added = cJSON_AddItemToObject(root, "values", farr);

    // step 4: Operate - inspect array size and specific items, check rawness and retrieve string value
    arr_size = cJSON_GetArraySize(farr);
    elem0 = cJSON_GetArrayItem(farr, 0);
    elem_str = cJSON_GetArrayItem(farr, 3);
    elem_raw = cJSON_GetArrayItem(farr, 4);
    is_raw = cJSON_IsRaw(elem_raw);
    str_val = cJSON_GetStringValue(elem_str);

    // step 5: Validate - produce an unformatted print and derive a simple validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = arr_size + (int)(str_val != (const char *)0) + (int)is_raw + (int)(elem0 != (cJSON *)0) + (int)added;

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)str_val;
    (void)elem0;
    (void)elem_str;
    (void)elem_raw;
    (void)arr_size;
    (void)is_raw;
    (void)added;
    // API sequence test completed successfully
    return 66;
}