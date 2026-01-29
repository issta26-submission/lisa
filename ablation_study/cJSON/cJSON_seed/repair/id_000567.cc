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
//<ID> 567
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[4] = {1.5, 2.5, 3.5, 4.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateDoubleArray(nums, 4);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "original_count", 4);

    // step 2: Configure
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *first = cJSON_GetArrayItem(got_values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_doubled", first_val * 2.0);
    cJSON_DeleteItemFromArray(got_values, 1);

    // step 3: Operate and Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_values = cJSON_GetObjectItemCaseSensitive(dup, "values");
    cJSON *second_dup = cJSON_GetArrayItem(dup_values, 1);
    double second_val = cJSON_GetNumberValue(second_dup);
    cJSON_AddNumberToObject(dup, "second_value_copied", second_val);
    char *out = cJSON_PrintUnformatted(dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}