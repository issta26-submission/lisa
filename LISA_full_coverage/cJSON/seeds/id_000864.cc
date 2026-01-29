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
//<ID> 864
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float numbers[3];
    numbers[0] = 1.5f;
    numbers[1] = 2.5f;
    numbers[2] = -3.0f;
    cJSON *float_array = cJSON_CreateFloatArray(numbers, 3);
    cJSON_AddItemToObject(root, "floats", float_array);

    // step 2: Configure
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed_ok;
    const char *json_text = " { \"config\" : { \"threshold\": 7.25 }, \"key\": 42 } extra";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 0);

    // step 3: Operate
    cJSON *config = cJSON_GetObjectItem(parsed, "config");
    cJSON *threshold_item = cJSON_GetObjectItem(config, "threshold");
    double threshold = cJSON_GetNumberValue(threshold_item);
    cJSON *first_element = cJSON_GetArrayItem(float_array, 0);
    double current = cJSON_GetNumberValue(first_element);
    cJSON_SetNumberHelper(first_element, current + threshold);
    cJSON *f = cJSON_AddFalseToObject(root, "enabled");
    (void)f;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}