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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(nested, "value", num_item);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw", "{\"raw_key\":7}");

    // step 2: Configure
    float floats_buf[3];
    floats_buf[0] = 1.1f;
    floats_buf[1] = 2.2f;
    floats_buf[2] = 3.3f;
    cJSON *float_array = cJSON_CreateFloatArray(floats_buf, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON *nested_ref = cJSON_CreateObjectReference(nested);
    cJSON_AddItemToObject(root, "nested_ref", nested_ref);

    // step 3: Operate and Validate
    cJSON *retrieved_num = cJSON_GetObjectItem(nested, "value");
    double numeric_value = cJSON_GetNumberValue(retrieved_num);
    cJSON_bool nested_is_obj = cJSON_IsObject(nested);
    cJSON_bool raw_is_obj = cJSON_IsObject(raw_item);
    int float_count = cJSON_GetArraySize(float_array);
    int summary = (int)numeric_value + (int)nested_is_obj + (int)raw_is_obj + float_count + root->type + nested->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}