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
//<ID> 1485
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem_raw = (cJSON *)0;
    cJSON *raw_node = (cJSON *)0;
    float nums[3];
    int arr_size = 0;
    cJSON_bool added_ok = 0;
    cJSON_bool raw_flag = 0;
    const char *raw_text = (const char *)0;
    char *str_val = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - prepare float array data and create JSON structures
    nums[0] = 1.5f;
    nums[1] = 2.5f;
    nums[2] = 3.5f;
    root = cJSON_CreateObject();
    arr = cJSON_CreateFloatArray((const float *)nums, 3);
    raw_node = cJSON_CreateRaw("{\"raw\":true}");

    // step 3: Configure - attach raw node into the array and add array to root
    cJSON_AddItemToArray(arr, raw_node);
    added_ok = cJSON_AddItemToObject(root, "floats", arr);

    // step 4: Operate - inspect array, retrieve elements, and query types/strings
    arr_size = cJSON_GetArraySize(arr);
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    elem_raw = cJSON_GetArrayItem(arr, 3);
    raw_flag = cJSON_IsRaw(elem_raw);
    raw_text = cJSON_GetStringValue(elem_raw);
    str_val = (char *)raw_text;

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)(arr_size == 4) + (int)(str_val != (char *)0) + (int)raw_flag + (int)(elem0 != (cJSON *)0) + (int)added_ok;

    // step 6: Cleanup - free JSON structures and silence unused variable warnings
    cJSON_Delete(root);
    (void)elem1;
    (void)validation_score;
    (void)arr_size;
    (void)str_val;
    (void)raw_text;
    (void)raw_node;
    (void)elem_raw;
    (void)elem0;
    (void)added_ok;
    // API sequence test completed successfully
    return 66;
}