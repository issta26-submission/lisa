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
//<ID> 291
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(arr, num1);
    cJSON *bool_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, bool_false);
    cJSON *inner_obj = cJSON_CreateObject();
    cJSON_AddTrueToObject(inner_obj, "ready");
    cJSON_AddNumberToObject(inner_obj, "count", 2.0);
    cJSON_AddItemToArray(arr, inner_obj);

    // step 2: Configure
    const char *raw_payload = "{\"raw\":true}";
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw_payload", raw_payload);
    cJSON *replacement_num = cJSON_CreateNumber(42.0);
    cJSON_bool replaced_flag = cJSON_ReplaceItemInArray(arr, 0, replacement_num);
    cJSON_AddNumberToObject(root, "replace_result", (double)replaced_flag);

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool first_is_obj = cJSON_IsObject(first_item);
    cJSON_bool second_is_false = cJSON_IsFalse(second_item);
    cJSON_AddBoolToObject(root, "first_is_object", first_is_obj);
    cJSON_AddBoolToObject(root, "second_is_false", second_is_false);
    char *dump = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(dump);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}