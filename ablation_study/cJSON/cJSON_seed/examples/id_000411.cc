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
//<ID> 411
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *num3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);
    cJSON_AddItemToArray(items, num3);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToArray(items, maybe_null);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(items);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(items, 1, replacement);
    cJSON *extra = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(items, extra);
    int size_after = cJSON_GetArraySize(items);

    // step 3: Operate & Validate
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_bool null_is_null = cJSON_IsNull(maybe_null);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "has_items=%d|null=%d|before=%d|after=%d", (int)has_items, (int)null_is_null, size_before, size_after);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}