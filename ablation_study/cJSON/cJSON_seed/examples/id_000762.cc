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
//<ID> 762
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *array = cJSON_CreateDoubleArray(nums, 3);
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", array);
    cJSON_AddItemToObject(root, "meta", child);
    cJSON_AddNumberToObject(child, "count", 3.0);
    cJSON_AddItemToObject(root, "nullable", null_item);
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    cJSON *second = cJSON_GetArrayItem(parsed_values, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *parsed_nullable = cJSON_GetObjectItem(parsed, "nullable");
    cJSON_bool is_null = cJSON_IsNull(parsed_nullable);
    (void)second_val;
    (void)is_null;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}