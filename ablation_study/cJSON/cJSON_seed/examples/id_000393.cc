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
//<ID> 393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const double nums[] = {1.5, 2.5, 3.5};
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "optional", maybe_null);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double old_val = cJSON_GetNumberValue(second_item);
    double new_val = cJSON_SetNumberHelper(second_item, old_val * 2.0);
    cJSON *sum_item = cJSON_CreateNumber(old_val + new_val);
    cJSON_AddItemToObject(root, "sum_values", sum_item);

    // step 3: Operate & Validate
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "flag=%d|old=%.2f|new=%.2f", (int)flag_is_true, old_val, new_val);
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