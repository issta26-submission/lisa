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
//<ID> 290
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
    cJSON_AddItemToArray(arr, num0);
    cJSON *bool_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, bool_false);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(9.99);
    cJSON_bool replace_ok = cJSON_ReplaceItemInArray(arr, 0, replacement);
    cJSON *raw_added = cJSON_AddRawToObject(root, "config", "{\"enabled\":false,\"count\":3}");

    // step 3: Operate and Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_false = cJSON_IsFalse(second_item);
    cJSON_AddBoolToObject(root, "replace_ok", replace_ok);
    cJSON_AddBoolToObject(root, "root_is_object", root_is_obj);
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