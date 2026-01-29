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
//<ID> 453
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"meta\":{\"name\":\"sample\"}}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);

    // step 2: Configure
    float nums[] = {3.14f, 6.28f, 9.42f};
    cJSON *float_arr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "float_values", float_arr);
    cJSON *enabled_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled_flag);

    // step 3: Operate
    cJSON *retrieved_array = cJSON_GetObjectItem(root, "float_values");
    int arr_size = cJSON_GetArraySize(retrieved_array);
    (void)arr_size;
    cJSON *detached_item = cJSON_DetachItemFromArray(retrieved_array, 1);
    double detached_value = cJSON_GetNumberValue(detached_item);
    (void)detached_value;

    // step 4: Validate & Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}