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
//<ID> 2204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[4] = {1.1f, 2.2f, 3.3f, 4.4f};
    cJSON *floats = cJSON_CreateFloatArray(nums, 4);
    cJSON_AddItemToObject(root, "floats", floats);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n2);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, n3);
    cJSON *flag = cJSON_AddFalseToObject(root, "active");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(25.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 1, replacement);
    (void)replaced;
    (void)flag;

    // step 3: Operate and Validate
    cJSON *floats_obj = cJSON_GetObjectItem(root, "floats");
    cJSON *third_float = cJSON_GetArrayItem(floats_obj, 2);
    double extracted = cJSON_GetNumberValue(third_float);
    cJSON *extra = cJSON_CreateNumber(extracted + 100.0);
    cJSON_AddItemToArray(items, extra);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
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