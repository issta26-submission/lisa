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
//<ID> 457
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1.5,2.5,3.5],\"flag\":false}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    double first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    cJSON_bool original_flag_truth = cJSON_IsTrue(flag_item);

    // step 2: Configure
    float new_nums[3];
    new_nums[0] = 4.5f;
    new_nums[1] = 5.5f;
    new_nums[2] = 6.5f;
    cJSON *float_arr = cJSON_CreateFloatArray(new_nums, 3);
    cJSON_AddItemToObject(root, "float_copy", float_arr);
    cJSON *created_bool = cJSON_CreateBool(original_flag_truth ? 1 : 0);
    cJSON_AddItemToObject(root, "created_bool", created_bool);

    // step 3: Operate
    cJSON *detached_item = cJSON_DetachItemFromArray(values, 1);
    int remaining = cJSON_GetArraySize(values);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    (void)first_value;
    (void)remaining;
    (void)original_flag_truth;
    return 66;
    // API sequence test completed successfully
}