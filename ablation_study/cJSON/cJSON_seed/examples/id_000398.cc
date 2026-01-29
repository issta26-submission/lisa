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
//<ID> 398
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = { 1.1, 2.2, 3.3 };
    cJSON *values_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON_AddItemToObject(root, "flag", flag_true);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(values_array, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON_bool flag_state = cJSON_IsTrue(flag_true);

    // step 3: Operate & Validate
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "flag=%d|second=%.2f", (int)flag_state, second_val);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_item);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}