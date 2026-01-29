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
//<ID> 690
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *strings[] = {"first", "second", "third"};
    cJSON *str_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "items", str_array);
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", nullable);
    cJSON *flag = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    cJSON *arr_ref = cJSON_GetObjectItem(root, "items");
    cJSON_bool arr_is_array = cJSON_IsArray(arr_ref);
    cJSON *first = cJSON_GetArrayItem(arr_ref, 0);
    const char *first_value = cJSON_GetStringValue(first);
    cJSON *raw_item = cJSON_CreateRaw("raw_payload");
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddItemToObject(root, "raw_candidate", raw_item);
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON_ReplaceItemInObject(root, "nullable", replacement);

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    int arr_size = cJSON_GetArraySize(arr_ref);
    cJSON *arr_copy = cJSON_Duplicate(arr_ref, 1);
    cJSON_AddItemToObject(root, "items_copy", arr_copy);
    (void)flag;
    (void)first_value;
    (void)arr_is_array;
    (void)raw_is_raw;
    (void)arr_size;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}