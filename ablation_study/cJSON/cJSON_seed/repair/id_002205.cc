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
//<ID> 2205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[4] = {1.5f, 2.5f, 3.5f, 4.5f};
    cJSON *floatArray = cJSON_CreateFloatArray(nums, 4);
    cJSON_AddItemToObject(root, "floats", floatArray);
    cJSON *arrayB = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arrayB", arrayB);

    // step 2: Configure
    cJSON_AddFalseToObject(root, "enabled");
    cJSON *replacement = cJSON_CreateNumber(9.25);
    cJSON_ReplaceItemInArray(floatArray, 1, replacement);
    cJSON *extra = cJSON_CreateNumber(7.75);
    cJSON_AddItemToArray(arrayB, extra);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "floats");
    cJSON *second_item = cJSON_GetArrayItem(retrieved, 1);
    double extracted = cJSON_GetNumberValue(second_item);
    cJSON *derived = cJSON_CreateNumber(extracted + 1.0);
    cJSON_AddItemToArray(arrayB, derived);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}