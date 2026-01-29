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
//<ID> 1520
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("zero");
    cJSON *num_item = cJSON_CreateNumber(1.0);
    cJSON *bool_item = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "flag", bool_item);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 0, replacement);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool first_is_str = cJSON_IsString(first);
    cJSON_bool second_is_num = cJSON_IsNumber(second);
    const char *first_text = cJSON_GetStringValue(first);
    double second_val = cJSON_GetNumberValue(second);
    char *printed = cJSON_PrintUnformatted(root);
    (void)first_is_str;
    (void)second_is_num;
    (void)first_text;
    (void)second_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}