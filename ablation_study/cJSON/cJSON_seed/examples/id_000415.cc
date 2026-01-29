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
//<ID> 415
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, maybe_null);
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "size=%d|first=%.0f", arr_size, first_val);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);

    // step 3: Operate & Validate
    cJSON *third_item = cJSON_GetArrayItem(arr, 2);
    cJSON_bool third_is_null = cJSON_IsNull(third_item);
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    char check_buf[64];
    memset(check_buf, 0, sizeof(check_buf));
    sprintf(check_buf, "has_data=%d|null3=%d", (int)has_data, (int)third_is_null);
    cJSON *check = cJSON_CreateString(check_buf);
    cJSON_AddItemToObject(root, "check", check);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}