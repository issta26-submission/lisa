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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.5);
    float floats_arr[3] = {1.25f, 2.5f, 3.75f};
    cJSON *float_array = cJSON_CreateFloatArray(floats_arr, 3);
    const char *raw_json = "{\"inner\": 7}";
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw", raw_json);

    // step 2: Configure
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "floats", float_array);

    // step 3: Operate and Validate
    double number_value = cJSON_GetNumberValue(num);
    cJSON_bool root_is_object = cJSON_IsObject(root);
    int float_count = cJSON_GetArraySize(float_array);
    int summary = (int)number_value + (int)root_is_object + float_count + raw_item->type;
    (void)summary;
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}