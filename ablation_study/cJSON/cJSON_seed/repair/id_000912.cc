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
//<ID> 912
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
    cJSON *first_num = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, first_num);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToArray(items, flag);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *label = cJSON_CreateString("old_label");
    cJSON_AddItemToObject(meta, "label", label);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON *detached_name = cJSON_DetachItemFromObject(root, "name");
    const char *old_name = cJSON_GetStringValue(detached_name);
    cJSON *new_label = cJSON_CreateString(old_name);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(meta, "label", new_label);
    (void)replaced;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(detached_name);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}