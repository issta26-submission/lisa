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
//<ID> 410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double init_vals[3] = {1.0, 2.0, 3.0};
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(init_vals[0]);
    cJSON *n1 = cJSON_CreateNumber(init_vals[1]);
    cJSON *n2 = cJSON_CreateNumber(init_vals[2]);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, maybe_null);
    int size_before = cJSON_GetArraySize(arr);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_null = cJSON_IsNull(second_item);

    // step 3: Operate & Validate
    char status_buf[192];
    memset(status_buf, 0, sizeof(status_buf));
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    int size_after = cJSON_GetArraySize(arr);
    sprintf(status_buf, "has_data=%d|size_before=%d|size_after=%d|first=%.0f|replaced=%d|second_is_null=%d",
            (int)has_data, size_before, size_after, first_val, (int)replaced, (int)second_is_null);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(7.7));

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}