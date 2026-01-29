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
//<ID> 135
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *tags[] = { "alpha", "beta", "gamma" };
    cJSON *tags_array = cJSON_CreateStringArray(tags, 3);
    float values[] = { 1.0f, -2.5f, 3.14f, 0.0f };
    cJSON *values_array = cJSON_CreateFloatArray(values, 4);
    cJSON_AddItemToObject(root, "tags", tags_array);
    cJSON_AddItemToObject(root, "values", values_array);

    // step 2: Configure
    cJSON *tags_array_copy = cJSON_CreateStringArray(tags, 3);
    cJSON *values_array_copy = cJSON_CreateFloatArray(values, 4);
    cJSON_bool tags_equal = cJSON_Compare(tags_array, tags_array_copy, 1);
    cJSON_bool values_equal = cJSON_Compare(values_array, values_array_copy, 1);
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    char scratch[128];
    memset(scratch, 0, sizeof(scratch));
    int tags_count = cJSON_GetArraySize(tags_array);
    int values_count = cJSON_GetArraySize(values_array);
    int summary = (int)out[0] + tags_count + values_count + (int)tags_equal + (int)values_equal + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(tags_array_copy);
    cJSON_Delete(values_array_copy);

    // API sequence test completed successfully
    return 66;
}