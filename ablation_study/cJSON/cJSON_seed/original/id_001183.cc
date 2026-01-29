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
//<ID> 1183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_json = "{\"initial\":5, \"list\":[1,2,3]}";
    cJSON *parsed = cJSON_Parse(initial_json);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *shared = cJSON_CreateString("shared_value");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(arr, shared);
    cJSON_AddItemToObject(root, "referenced_array", arr);
    cJSON_bool arr_flag = cJSON_IsArray(arr);
    cJSON_AddBoolToObject(root, "referenced_array_is_array", arr_flag);
    cJSON_AddNullToObject(root, "maybe_null");

    // step 3: Operate
    cJSON *placeholder = cJSON_CreateNumber(99);
    cJSON_AddItemToObject(root, "to_replace", placeholder);
    cJSON *replacement = cJSON_CreateString("was_replaced");
    cJSON_ReplaceItemViaPointer(root, placeholder, replacement);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}