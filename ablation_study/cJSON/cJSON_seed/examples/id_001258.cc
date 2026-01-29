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
//<ID> 1258
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *ref = cJSON_CreateStringReference("first");
    cJSON_AddItemToArray(arr, ref);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(root, "derived", first_str);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "enabled");
    cJSON_AddItemToArray(arr, detached);

    // step 3: Operate & Validate
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_bool = cJSON_IsBool(second_item);
    cJSON_AddBoolToObject(root, "second_was_bool", second_is_bool);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}