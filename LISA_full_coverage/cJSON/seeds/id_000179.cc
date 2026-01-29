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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float numbers[3] = {1.5f, 2.25f, -4.75f};
    cJSON *float_array = cJSON_CreateFloatArray(numbers, 3);
    cJSON_AddItemToObject(root, "float_values", float_array);

    // step 2: Configure
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_metadata", "{\"version\": 1, \"note\": \"raw data\"}");

    // step 3: Operate and Validate
    cJSON *first_elem = cJSON_GetArrayItem(float_array, 0);
    double first_value = cJSON_GetNumberValue(first_elem);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    int array_size = cJSON_GetArraySize(float_array);
    int summary = (int)first_value + array_size + (int)root_is_obj + (root ? root->type : 0) + (raw_item ? raw_item->type : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}