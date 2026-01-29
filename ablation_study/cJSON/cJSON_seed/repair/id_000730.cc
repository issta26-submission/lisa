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
//<ID> 730
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"item\": 42, \"nested\": {\"value\": 3.14}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *orig_item = cJSON_GetObjectItem(root, "item");
    cJSON *nested = cJSON_GetObjectItem(root, "nested");

    // step 2: Configure
    cJSON *nested_dup = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "nested_copy", nested_dup);
    cJSON *new_item = cJSON_CreateNumber(100.0);
    cJSON_ReplaceItemViaPointer(root, orig_item, new_item);
    cJSON *nested_copy = cJSON_GetObjectItem(root, "nested_copy");
    cJSON *nested_value = cJSON_GetObjectItem(nested_copy, "value");
    cJSON *replacement_value = cJSON_CreateNumber(6.28);
    cJSON_ReplaceItemViaPointer(nested_copy, nested_value, replacement_value);

    // step 3: Operate and Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}