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
//<ID> 1255
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
    cJSON *meta = cJSON_CreateObject();
    cJSON *refstr = cJSON_CreateStringReference("alpha_ref");
    cJSON *refstr2 = cJSON_CreateStringReference("beta_ref");
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToArray(arr, refstr);
    cJSON_AddItemToArray(arr, refstr2);
    cJSON_AddStringToObject(root, "owner", "bob");
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    const char *first_value = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(root, "first_item_value", first_value);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON_Delete(detached_meta);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}