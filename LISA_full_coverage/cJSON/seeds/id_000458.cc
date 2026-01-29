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
//<ID> 458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1.0,2.5,3.75],\"flag\":false}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *values = cJSON_GetObjectItem(root, "values");

    // step 2: Configure
    float extra_vals[2] = { 4.5f, 5.25f };
    cJSON *more_values = cJSON_CreateFloatArray(extra_vals, 2);
    cJSON_AddItemToObject(root, "more_values", more_values);
    cJSON *added_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "added_flag", added_flag);

    // step 3: Operate
    int size_before = cJSON_GetArraySize(values);
    cJSON *detached_item = cJSON_DetachItemFromArray(values, 1);
    cJSON_AddItemToArray(more_values, detached_item);
    int size_after = cJSON_GetArraySize(values);
    (void)size_before;
    (void)size_after;
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}