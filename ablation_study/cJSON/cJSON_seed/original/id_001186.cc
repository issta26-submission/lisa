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
//<ID> 1186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[1,2,3], \"obj\": {\"k\":\"v\"}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "wrapped", parsed);

    // step 2: Configure
    cJSON *arr_item = cJSON_GetObjectItem(parsed, "arr");
    cJSON_bool arr_is_array = cJSON_IsArray(arr_item);
    cJSON_AddBoolToObject(root, "arr_is_array", arr_is_array);
    cJSON *new_array = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(new_array, arr_item);
    cJSON_AddItemToObject(root, "ref_array", new_array);

    // step 3: Operate
    cJSON *old_obj = cJSON_GetObjectItem(parsed, "obj");
    cJSON *replacement = cJSON_CreateString("replaced_obj");
    cJSON_ReplaceItemViaPointer(parsed, old_obj, replacement);
    cJSON_AddNullToObject(root, "added_null");

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}