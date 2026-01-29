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
//<ID> 298
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
    cJSON *num0 = cJSON_CreateNumber(1.23);
    cJSON *num1 = cJSON_CreateNumber(4.56);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON *bool_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, bool_item);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw", "{\"raw_key\":true}");
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddNumberToObject(nested, "inner_value", 7.0);

    // step 3: Operate and Validate
    cJSON_bool nested_is_object = cJSON_IsObject(nested);
    cJSON_AddNumberToObject(root, "nested_is_object", (double)nested_is_object);
    cJSON_bool was_false = cJSON_IsFalse(bool_item);
    cJSON_AddBoolToObject(root, "was_false", was_false);
    int count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "item_count", (double)count);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_bool replacement_is_object = cJSON_IsObject(replacement);
    cJSON_AddBoolToObject(root, "replacement_is_object", replacement_is_object);
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}