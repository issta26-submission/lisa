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
    const char *json_text = "{\"arr\":[1,false,null],\"flag\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(created_arr, n1);
    cJSON_AddItemToArray(created_arr, n2);
    cJSON_AddItemToObject(root, "generated", created_arr);

    // step 2: Configure
    cJSON_AddBoolToObject(root, "ok", 1);
    cJSON_AddNullToObject(root, "missing");
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *first = cJSON_GetArrayItem(parsed_arr, 0);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(parsed_arr, first, replacement);

    // step 3: Operate & Validate
    cJSON *rep = cJSON_GetArrayItem(parsed_arr, 0);
    const char *rep_str = cJSON_GetStringValue(rep);
    cJSON_AddStringToObject(root, "replaced_value", rep_str);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}