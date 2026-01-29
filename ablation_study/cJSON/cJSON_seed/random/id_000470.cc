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
//<ID> 470
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
    cJSON *str_in_array = (cJSON *)0;
    cJSON *raw_in_array = (cJSON *)0;
    cJSON *label_str = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *first_value = (const char *)0;
    const char *label_value = (const char *)0;
    cJSON_bool first_is_raw = 0;
    cJSON_bool second_is_raw = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_in_array = cJSON_CreateString("hello");
    raw_in_array = cJSON_CreateRaw("RAW_CONTENT");
    label_str = cJSON_CreateString("example_label");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_in_array);
    cJSON_AddItemToArray(arr, raw_in_array);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "label", label_str);
    cJSON_AddNullToObject(root, "empty");

    // step 4: Operate
    first_item = cJSON_GetArrayItem(arr, 0);
    second_item = cJSON_GetArrayItem(arr, 1);
    first_value = cJSON_GetStringValue(first_item);
    label_value = cJSON_GetStringValue(label_str);
    first_is_raw = cJSON_IsRaw(first_item);
    second_is_raw = cJSON_IsRaw(second_item);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    (void)first_value;
    (void)label_value;
    (void)first_is_raw;
    (void)second_is_raw;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}