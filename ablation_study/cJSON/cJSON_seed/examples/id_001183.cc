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
    const char *json_text = "{\"status\":false,\"value\":42,\"items\":[5,7,9]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *replacement = cJSON_CreateNumber(100.0);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(parsed, "items");
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON_ReplaceItemViaPointer(items, second_item, replacement);
    cJSON_AddNullToObject(root, "nullable");
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate & Validate
    cJSON *updated = cJSON_GetArrayItem(items, 1);
    double replaced_value = cJSON_GetNumberValue(updated);
    cJSON_AddNumberToObject(root, "replaced_value", replaced_value);
    cJSON *status = cJSON_GetObjectItem(parsed, "status");
    cJSON_bool was_false = cJSON_IsFalse(status);
    cJSON_AddBoolToObject(root, "status_was_false", was_false);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}