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
//<ID> 1182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"alpha\": 1, \"beta\": \"x\"}";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *shared = cJSON_CreateString("shared_value");
    cJSON_AddItemToObject(root, "shared", shared);
    cJSON_AddItemReferenceToArray(arr, shared);

    // step 2: Configure
    cJSON *alpha_item = cJSON_GetObjectItem(parsed, "alpha");
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemViaPointer(parsed, alpha_item, replacement);
    cJSON_AddNullToObject(root, "added_null");
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    cJSON_AddBoolToObject(root, "arr_is_array", arr_is_array);

    // step 3: Operate
    cJSON *beta_item = cJSON_GetObjectItem(parsed, "beta");
    const char *beta_str = cJSON_GetStringValue(beta_item);
    cJSON *beta_dup = cJSON_CreateString(beta_str);
    cJSON_AddItemToObject(root, "beta_dup", beta_dup);
    char *flat = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(flat);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}