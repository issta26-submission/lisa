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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    float numbers[4];
    numbers[0] = 1.5f;
    numbers[1] = 2.5f;
    numbers[2] = -3.25f;
    numbers[3] = 4.0f;
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateFloatArray(numbers, 4);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "name", "dataset");
    cJSON_AddNumberToObject(meta, "count", 4.0);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "status", "ok");
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate and Validate
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    int array_size = cJSON_GetArraySize(retrieved_values);
    cJSON *meta_obj = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_obj);
    cJSON *third_item = cJSON_GetArrayItem(retrieved_values, 2);
    double third_val = cJSON_GetNumberValue(third_item);
    cJSON *name_item = cJSON_GetObjectItem(meta_obj, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    char *pretty = cJSON_Print(root);
    char *buffered = cJSON_PrintBuffered(root, 128, 1);
    int summary = (int)array_size + (int)meta_is_obj + (int)third_val + (int)name_str[0];
    (void)summary;
    cJSON_free(pretty);
    cJSON_free(buffered);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}