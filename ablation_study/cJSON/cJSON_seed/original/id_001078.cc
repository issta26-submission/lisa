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
//<ID> 1078
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"sensor\",\"values\":[1,2,3],\"threshold\":10}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *values_item = cJSON_GetObjectItem(root, "values");
    cJSON *threshold_item = cJSON_GetObjectItem(root, "threshold");
    double old_threshold = cJSON_GetNumberValue(threshold_item);
    cJSON *new_threshold = cJSON_CreateNumber(old_threshold + 5.5);

    // step 3: Operate
    double new_values[] = {4.5, 5.5, 6.5};
    cJSON *new_array = cJSON_CreateDoubleArray(new_values, 3);
    cJSON_bool replaced_values = cJSON_ReplaceItemInObjectCaseSensitive(root, "values", new_array);
    cJSON_bool replaced_threshold = cJSON_ReplaceItemInObjectCaseSensitive(root, "threshold", new_threshold);
    cJSON *active_item = cJSON_AddTrueToObject(root, "active");

    // step 4: Validate & Cleanup
    int updated_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "values"));
    cJSON *first = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "values"), 0);
    double first_value = cJSON_GetNumberValue(first);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}