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
//<ID> 1216
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
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *child_num = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(child, "child_val", child_num);

    // step 3: Operate and Validate
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToArray(arr, dup_child);
    cJSON *detached_vals = cJSON_DetachItemFromObject(root, "values");
    cJSON_AddItemToObject(child, "moved_values", detached_vals);
    double child_v = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "child_val"));
    double first_arr_v = cJSON_GetNumberValue(cJSON_GetArrayItem(detached_vals, 0));
    double final_value = child_v + first_arr_v;
    cJSON *final_num = cJSON_CreateNumber(final_value);
    cJSON_AddItemToObject(root, "final", final_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}