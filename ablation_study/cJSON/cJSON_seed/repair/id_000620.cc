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
//<ID> 620
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(2.5));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(7.5));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(10.0));

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "scale", 2.0);
    cJSON_AddNullToObject(meta, "optional");
    cJSON *enabled = cJSON_AddTrueToObject(meta, "enabled");

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(numbers);
    cJSON *n0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *n1 = cJSON_GetArrayItem(numbers, 1);
    cJSON *n2 = cJSON_GetArrayItem(numbers, 2);
    double v0 = cJSON_GetNumberValue(n0);
    double v1 = cJSON_GetNumberValue(n1);
    double v2 = cJSON_GetNumberValue(n2);
    double sum = v0 + v1 + v2;
    double average = sum / (double)count;
    cJSON_AddNumberToObject(root, "average", average);
    cJSON *scale_item = cJSON_GetObjectItem(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    double scaled_average = average * scale;
    cJSON_AddNumberToObject(root, "average_scaled", scaled_average);
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled);
    cJSON_AddNumberToObject(root, "enabled_is_bool", (double)enabled_is_bool);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}