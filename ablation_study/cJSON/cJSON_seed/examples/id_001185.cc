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
//<ID> 1185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"flags\":[true,null,false],\"meta\":{}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *generated_arr = cJSON_CreateArray();
    cJSON *g_t = cJSON_CreateTrue();
    cJSON *g_n = cJSON_CreateNull();
    cJSON *g_f = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(generated_arr, g_t);
    cJSON_AddItemToArray(generated_arr, g_n);
    cJSON_AddItemToArray(generated_arr, g_f);
    cJSON_AddItemToObject(root, "generated", generated_arr);
    cJSON_AddNullToObject(root, "empty_slot");
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate & Validate
    cJSON *flags = cJSON_GetObjectItem(parsed, "flags");
    cJSON *third_flag = cJSON_GetArrayItem(flags, 2);
    cJSON *replacement = cJSON_CreateNumber(123.0);
    cJSON_bool did_replace = cJSON_ReplaceItemViaPointer(flags, third_flag, replacement);
    (void)did_replace;
    cJSON *first_generated = cJSON_GetArrayItem(generated_arr, 0);
    (void)first_generated;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}