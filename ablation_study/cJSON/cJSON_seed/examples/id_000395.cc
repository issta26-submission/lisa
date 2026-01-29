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
//<ID> 395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", maybe_null);
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *num_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", num_array);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(num_array, true_item);

    // step 2: Configure
    cJSON *array_copy = cJSON_Duplicate(num_array, 1);
    cJSON_bool arrays_equal = cJSON_Compare(num_array, array_copy, 1);

    // step 3: Operate & Validate
    cJSON *retrieved_true = cJSON_GetArrayItem(num_array, 3);
    cJSON_bool is_true = cJSON_IsTrue(retrieved_true);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "is_true=%d|arrays_equal=%d|len=%d", (int)is_true, (int)arrays_equal, 3);
    cJSON *status_item = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_item);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(array_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}