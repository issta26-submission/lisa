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
//<ID> 475
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, true_item);
    cJSON_AddItemToArray(items, false_item);
    cJSON_AddStringToObject(root, "status", "initialized");
    cJSON *raw = cJSON_CreateRaw("{\"note\":\"raw payload\"}");
    cJSON_AddItemToObject(root, "raw", raw);

    // step 3: Operate & Validate
    cJSON_bool first_is_true = cJSON_IsTrue(cJSON_GetArrayItem(items, 0));
    cJSON_bool second_is_false = cJSON_IsFalse(cJSON_GetArrayItem(items, 1));
    cJSON_DeleteItemFromArray(items, 1);
    int size_after_delete = cJSON_GetArraySize(items);
    char info[256];
    memset(info, 0, sizeof(info));
    sprintf(info, "ver=%s|first_true=%d|second_false_before_delete=%d|size_after_delete=%d",
            version ? version : "unknown", (int)first_is_true, (int)second_is_false, size_after_delete);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}