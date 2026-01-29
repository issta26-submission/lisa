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
//<ID> 796
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    const char *json_text = "{\"imported\": [10, 20, 30]}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON_AddTrueToObject(settings, "enabled");
    cJSON_AddNumberToObject(settings, "threshold", 4.2);
    cJSON *import_array = cJSON_GetObjectItem(parsed, "imported");
    cJSON *first_import = cJSON_GetArrayItem(import_array, 0);
    cJSON *first_dup = cJSON_Duplicate(first_import, 1);
    cJSON_AddItemToArray(items, first_dup);
    cJSON_AddItemToArray(items, cJSON_CreateString("extra"));

    // step 3: Operate and Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_bool dup_invalid = cJSON_IsInvalid(first_dup);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    (void)dup_invalid;
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}