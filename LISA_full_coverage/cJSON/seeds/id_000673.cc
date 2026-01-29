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
//<ID> 673
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *first_str = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, first_str);
    cJSON *old_entry = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(root, "replace_me", old_entry);
    cJSON *enabled_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled_flag);

    // step 2: Configure
    cJSON *new_entry = cJSON_CreateString("new_value");
    cJSON_ReplaceItemInObject(root, "replace_me", new_entry);

    // step 3: Operate
    cJSON *arr_first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_string = cJSON_IsString(arr_first);
    cJSON_AddBoolToObject(root, "first_is_string", first_is_string);
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    cJSON_AddBoolToObject(root, "enabled_is_bool", enabled_is_bool);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "items_count", (double)count);

    // step 4: Validate & Cleanup
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}