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
//<ID> 391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    const double nums[3] = {1.5, 2.5, 3.5};
    cJSON *num_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON *null_item = cJSON_CreateNull();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(root, num_array);
    cJSON_AddItemToArray(root, null_item);
    cJSON_AddItemToArray(root, true_item);

    // step 2: Configure
    cJSON *retrieved_array = cJSON_GetArrayItem(root, 0);
    cJSON *second_number_item = cJSON_GetArrayItem(retrieved_array, 1);
    double second_value = cJSON_GetNumberValue(second_number_item);
    cJSON *offset_number = cJSON_CreateNumber(second_value + 10.0);
    cJSON_AddItemToArray(root, offset_number);

    // step 3: Operate & Validate
    cJSON *bool_item = cJSON_GetArrayItem(root, 2);
    cJSON_bool was_true = cJSON_IsTrue(bool_item);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "true=%d|second=%.2f|offset=%.2f", (int)was_true, second_value, cJSON_GetNumberValue(offset_number));
    cJSON *status_str = cJSON_CreateString(status_buf);
    cJSON_AddItemToArray(root, status_str);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToArray(root, snapshot_str);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}