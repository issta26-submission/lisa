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
//<ID> 2203
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
    float float_vals[3] = {1.5f, 2.5f, 3.5f};
    cJSON *float_array = cJSON_CreateFloatArray(float_vals, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON *flag = cJSON_AddFalseToObject(root, "enabled");
    (void)flag;
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *i0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, i0);
    cJSON *i1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, i1);
    cJSON *i2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, i2);
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 1, replacement);
    (void)replaced;

    // step 3: Operate and Validate
    cJSON *got_floats = cJSON_GetObjectItem(root, "floats");
    int float_count = cJSON_GetArraySize(got_floats);
    cJSON *first_float = cJSON_GetArrayItem(got_floats, 0);
    double first_value = cJSON_GetNumberValue(first_float);
    cJSON *derived = cJSON_CreateNumber(first_value + 0.75);
    cJSON_AddItemToArray(items, derived);
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
    (void)float_count;
    (void)first_value;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}