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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"original\",\"value\":42,\"remove_me\":\"bye\"}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *new_name = cJSON_CreateString("replaced");

    // step 2: Configure
    cJSON_bool replaced_name = cJSON_ReplaceItemInObject(root, "name", new_name);
    cJSON *value_node = cJSON_GetObjectItem(root, "value");
    double orig_val = cJSON_GetNumberValue(value_node);
    cJSON *new_value = cJSON_CreateNumber(orig_val + 1.0);
    cJSON_bool replaced_value = cJSON_ReplaceItemInObject(root, "value", new_value);

    // step 3: Operate and Validate
    cJSON *name_node = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_node);
    double value_after = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "remove_me");
    cJSON *removed_check = cJSON_GetObjectItem(root, "remove_me");
    double summary = orig_val + value_after + (double)replaced_name + (double)replaced_value + (removed_check ? 0.0 : 0.0);
    (void)name_str;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}