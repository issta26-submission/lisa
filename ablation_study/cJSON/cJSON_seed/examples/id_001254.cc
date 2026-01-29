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
//<ID> 1254
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "static", "static_value");
    cJSON_AddBoolToObject(child, "flag", 1);
    cJSON *arr = cJSON_CreateArray();
    cJSON *sref1 = cJSON_CreateStringReference("one");
    cJSON_AddItemToArray(arr, sref1);
    cJSON *sref2 = cJSON_CreateStringReference("two");
    cJSON_AddItemToArray(arr, sref2);
    cJSON_AddItemToObject(child, "list", arr);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    char *second_str = cJSON_GetStringValue(second_item);
    cJSON_AddStringToObject(root, "second", second_str);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(child, "static");
    cJSON_AddItemToObject(root, "detached", detached);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}