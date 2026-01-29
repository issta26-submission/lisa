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
//<ID> 813
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *items_arr[] = { "alpha", "beta", "gamma" };
    cJSON *items_array = cJSON_CreateStringArray(items_arr, 3);
    cJSON_AddItemToObject(root, "items", items_array);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *active = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "active", active);
    cJSON_AddNullToObject(root, "missing");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *owner = cJSON_CreateString("tester");
    cJSON_AddItemToObject(meta, "owner", owner);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *second_item = cJSON_GetArrayItem(parsed_items, 1);
    const char *second_value = cJSON_GetStringValue(second_item);
    cJSON *copied = cJSON_CreateString(second_value);
    cJSON_AddItemToObject(parsed, "copied_second", copied);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}