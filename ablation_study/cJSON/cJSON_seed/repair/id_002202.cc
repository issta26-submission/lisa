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
//<ID> 2202
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n2);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, n3);
    cJSON *replacement = cJSON_CreateNumber(25.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 1, replacement);
    (void)replaced;
    cJSON *false_flag = cJSON_AddFalseToObject(meta, "enabled");
    (void)false_flag;

    // step 3: Operate and Validate
    cJSON *fetched_floats = cJSON_GetObjectItem(root, "floats");
    cJSON *third = cJSON_GetArrayItem(fetched_floats, 2);
    double extracted = cJSON_GetNumberValue(third);
    cJSON *computed = cJSON_CreateNumber(extracted + 4.0);
    cJSON_AddItemToArray(items, computed);
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