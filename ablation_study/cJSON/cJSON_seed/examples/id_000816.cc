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
//<ID> 816
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *items[] = { "alpha", "beta", "gamma" };
    cJSON *array_item = cJSON_CreateStringArray(items, 3);
    cJSON_AddItemToObject(root, "items", array_item);
    cJSON_AddNullToObject(root, "optional_field");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON *bool_item = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "active", bool_item);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *owner_array = cJSON_CreateStringArray((const char *const[]){"ownerA","ownerB"}, 2);
    cJSON_AddItemToObject(meta, "owners", owner_array);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *second_item = cJSON_GetArrayItem(parsed_items, 1);
    const char *second_value = cJSON_GetStringValue(second_item);
    cJSON *derived_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(parsed, "derived_flag", derived_bool);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}