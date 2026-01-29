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
//<ID> 419
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *init_vals[3] = { "one", "two", "three" };
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateString(init_vals[0]);
    cJSON *item1 = cJSON_CreateString(init_vals[1]);
    cJSON *item2 = cJSON_CreateString(init_vals[2]);
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", maybe_null);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("TWO_REPLACED");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *extra_num = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(arr, extra_num);

    // step 3: Operate & Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_bool optional_is_null = cJSON_IsNull(maybe_null);
    const char *second_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    char status_buf[192];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "size=%d|has_items=%d|optional_null=%d|second=%s", arr_size, (int)has_items, (int)optional_is_null, second_val ? second_val : "null");
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