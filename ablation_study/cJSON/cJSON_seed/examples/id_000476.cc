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
//<ID> 476
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
    cJSON_AddStringToObject(root, "name", "cjson_api_sequence");
    cJSON_AddStringToObject(root, "version", version ? version : "unknown");

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *raw = cJSON_CreateRaw("{\"raw_key\":123}");
    cJSON_AddItemToArray(items, raw);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, str_item);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, true_item);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, false_item);

    // step 3: Operate & Validate
    cJSON_bool true_check = cJSON_IsTrue(true_item);
    cJSON_bool false_check = cJSON_IsFalse(false_item);
    int size_before = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromArray(items, 1);
    int size_after = cJSON_GetArraySize(items);
    cJSON *snapshot_obj = cJSON_Duplicate(root, 1);
    char *snapshot = cJSON_PrintUnformatted(snapshot_obj);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(snapshot_obj);
    cJSON_Delete(root);
    (void)true_check;
    (void)false_check;
    (void)size_before;
    (void)size_after;
    // API sequence test completed successfully
    return 66;
}