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
//<ID> 416
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int init_values[3] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateIntArray(init_values, 3);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *label = cJSON_CreateString("extra");
    cJSON_AddItemToArray(items, label);
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "answer", answer);

    // step 3: Operate & Validate
    int size_before = cJSON_GetArraySize(items);
    cJSON *append_val = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(items, append_val);
    int size_after = cJSON_GetArraySize(items);
    cJSON *replacement = cJSON_CreateNumber(123.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 1, replacement);
    cJSON_bool has_maybe = cJSON_HasObjectItem(root, "maybe");
    cJSON_bool maybe_is_null = cJSON_IsNull(maybe_null);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "has_maybe=%d|maybe_is_null=%d|size_before=%d|size_after=%d|replaced=%d",
        (int)has_maybe, (int)maybe_is_null, size_before, size_after, (int)replaced);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}