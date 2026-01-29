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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float sample_floats[3] = {1.1f, 2.2f, 3.3f};
    cJSON *float_array = cJSON_CreateFloatArray(sample_floats, 3);
    cJSON *number_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "float_values", float_array);
    cJSON_AddItemToObject(root, "answer", number_item);
    cJSON *raw_meta = cJSON_AddRawToObject(root, "metadata", "{\"type\":\"example\",\"version\":1}");

    // step 2: Configure
    (void)raw_meta; // ensure raw_meta variable is considered part of the flow
    cJSON_bool root_is_obj = cJSON_IsObject(root);

    // step 3: Operate and Validate
    double answer_val = cJSON_GetNumberValue(number_item);
    cJSON *first_float_item = cJSON_GetArrayItem(float_array, 0);
    double first_float_val = cJSON_GetNumberValue(first_float_item);
    double computed_sum = answer_val + first_float_val + (double)root_is_obj;
    char *printed = cJSON_PrintUnformatted(root);
    (void)computed_sum;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}