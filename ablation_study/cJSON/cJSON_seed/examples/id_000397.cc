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
//<ID> 397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *values = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *optional_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional_null);
    cJSON *second_item = cJSON_GetArrayItem(values, 1);

    // step 2: Configure
    double orig_second = cJSON_GetNumberValue(second_item);
    double factor = 1.0 + (double)cJSON_IsTrue(enabled);
    double updated_second = cJSON_SetNumberHelper(second_item, orig_second * factor);
    cJSON *values_copy = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "values_copy", values_copy);

    // step 3: Operate & Validate
    cJSON *copy_first = cJSON_GetArrayItem(values_copy, 0);
    double v0 = cJSON_GetNumberValue(copy_first);
    double computed_sum = v0 + updated_second;
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "orig=%.2f|updated=%.2f|sum=%.2f|enabled=%d", orig_second, updated_second, computed_sum, (int)cJSON_IsTrue(enabled));
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