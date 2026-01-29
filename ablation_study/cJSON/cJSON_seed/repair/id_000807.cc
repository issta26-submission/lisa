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
//<ID> 807
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, false_item);
    cJSON *str_ref = cJSON_CreateStringReference("initial_ref");
    cJSON_AddItemToObject(root, "label", str_ref);

    // step 2: Configure
    const char *json_text = "{\"label\":\"parsed_label\",\"extra\":123}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *detached_label = cJSON_DetachItemFromObject(parsed, "label");
    cJSON_AddItemToObject(root, "imported_label", detached_label);

    // step 3: Operate and Validate
    char *root_snapshot = cJSON_PrintUnformatted(root);
    char *parsed_snapshot = cJSON_PrintUnformatted(parsed);
    cJSON *extra_item = cJSON_GetObjectItem(parsed, "extra");
    double extra_val = cJSON_GetNumberValue(extra_item);
    cJSON_bool imported_is_string = cJSON_IsString(cJSON_GetObjectItem(root, "imported_label"));

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "label");
    cJSON_free(root_snapshot);
    cJSON_free(parsed_snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}