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
//<ID> 459
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"example\",\"numbers\":[1.1,2.2,3.3],\"meta\":{\"valid\":true}}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *valid = cJSON_GetObjectItem(meta, "valid");
    double orig_first = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 0));

    // step 2: Configure
    float new_floats_data[3];
    new_floats_data[0] = 4.4f;
    new_floats_data[1] = 5.5f;
    new_floats_data[2] = 6.6f;
    cJSON *new_float_array = cJSON_CreateFloatArray(new_floats_data, 3);
    cJSON_AddItemToObject(root, "new_floats", new_float_array);
    cJSON *bool_item = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "custom_flag", bool_item);
    cJSON *detached_holder = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "detached_items", detached_holder);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(numbers, 1);
    cJSON_AddItemToArray(detached_holder, detached);
    char *snapshot = cJSON_PrintUnformatted(root);
    int remaining_count = cJSON_GetArraySize(numbers);
    double moved_value = cJSON_GetNumberValue(cJSON_GetArrayItem(detached_holder, 0));
    (void)orig_first;
    (void)remaining_count;
    (void)moved_value;
    (void)valid;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}