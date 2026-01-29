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
//<ID> 2201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.5f, 2.5f, 3.5f};
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extras", extras);
    cJSON *enabled = cJSON_AddFalseToObject(root, "enabled");

    // step 2: Configure
    cJSON *floats_ref = cJSON_GetObjectItem(root, "floats");
    cJSON *replacement = cJSON_CreateNumber(9.5);
    cJSON_bool did_replace = cJSON_ReplaceItemInArray(floats_ref, 1, replacement);
    (void)did_replace;
    cJSON *initial_extra = cJSON_CreateNumber(7.25);
    cJSON_AddItemToArray(extras, initial_extra);

    // step 3: Operate and Validate
    cJSON *replaced_item = cJSON_GetArrayItem(floats_ref, 1);
    double extracted = cJSON_GetNumberValue(replaced_item);
    cJSON *derived = cJSON_CreateNumber(extracted + 0.75);
    cJSON_AddItemToArray(extras, derived);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}