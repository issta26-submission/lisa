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
//<ID> 866
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float numbers[4] = { 1.0f, 2.0f, 3.5f, 4.25f };
    cJSON *float_array = cJSON_CreateFloatArray(numbers, 4);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON *configured_num = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "configured_value", configured_num);

    // step 2: Configure
    cJSON_AddFalseToObject(root, "enabled");
    const char *json_text = " { \"value\": 2.71828 } trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 0);
    double parsed_value = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "value"));
    double previous_value = cJSON_SetNumberHelper(configured_num, parsed_value);
    (void)previous_value;

    // step 3: Operate
    int count = cJSON_GetArraySize(float_array);
    (void)count;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}