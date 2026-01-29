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
//<ID> 699
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *fruits[] = { "apple", "banana", "cherry" };
    cJSON *fruits_arr = cJSON_CreateStringArray(fruits, 3);
    cJSON_AddItemToObject(root, "fruits", fruits_arr);
    cJSON *active_flag = cJSON_AddBoolToObject(root, "active", 1);
    (void)active_flag;

    // step 2: Configure
    cJSON_bool arr_is_array = cJSON_IsArray(fruits_arr);
    cJSON_AddBoolToObject(root, "fruits_is_array", arr_is_array);
    cJSON *first_item = cJSON_GetArrayItem(fruits_arr, 0);
    cJSON_bool first_is_raw = cJSON_IsRaw(first_item);
    cJSON_AddBoolToObject(root, "first_is_raw", first_is_raw);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);

    // step 3: Operate
    cJSON *fruits_copy = cJSON_Duplicate(fruits_arr, 1);
    cJSON_AddItemToObject(root, "fruits_copy", fruits_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    int count = cJSON_GetArraySize(fruits_arr);
    cJSON_AddNumberToObject(root, "fruits_size", (double)count);
    cJSON *second_item = cJSON_GetArrayItem(fruits_arr, 1);
    const char *second_str = cJSON_GetStringValue(second_item);
    cJSON *second_copy = cJSON_CreateString(second_str);
    cJSON_AddItemToObject(root, "second_copy", second_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}