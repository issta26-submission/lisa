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
//<ID> 2207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddFalseToObject(meta, "active");
    float nums[3] = {1.5f, 2.5f, 3.5f};
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n2);
    cJSON *newn = cJSON_CreateNumber(15.0);
    cJSON_bool did_replace = cJSON_ReplaceItemInArray(items, 1, newn);
    cJSON_AddNumberToObject(root, "did_replace", (double)did_replace);

    // step 3: Operate and Validate
    cJSON *fetched_floats = cJSON_GetObjectItem(root, "floats");
    double first_float = cJSON_GetNumberValue(cJSON_GetArrayItem(fetched_floats, 0));
    cJSON_AddNumberToObject(root, "first_float", first_float);
    cJSON *fetched_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *active_item = cJSON_GetObjectItem(fetched_meta, "active");
    cJSON_AddNumberToObject(fetched_meta, "is_false_flag", (double)cJSON_IsFalse(active_item));
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