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
//<ID> 1221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"count\":3,\"values\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, (cJSON_bool)1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(parsed, "values");
    int nums[] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "int_array", int_array);

    // step 3: Operate & Validate
    cJSON *detached_values = cJSON_DetachItemFromObject(parsed, "values");
    cJSON_AddItemToObject(root, "migrated_values", detached_values);
    cJSON *count_item = cJSON_GetObjectItem(parsed, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *orig_count = cJSON_CreateNumber(count_val);
    cJSON_AddItemToObject(root, "original_count", orig_count);
    (void)values;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}