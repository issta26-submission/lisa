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
//<ID> 128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddStringToObject(root, "title", "double array example");

    // step 2: Configure
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *darr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "doubles", darr);
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON *old = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(config, "replace_me", old);
    cJSON *parsed = cJSON_Parse("{\"replace_me\":\"replaced\"}");
    cJSON *parsed_item = cJSON_GetObjectItem(parsed, "replace_me");
    cJSON *dup_item = cJSON_Duplicate(parsed_item, 1);

    // step 3: Operate and Validate
    cJSON_ReplaceItemViaPointer(config, old, dup_item);
    cJSON *second = cJSON_GetArrayItem(darr, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "second_double_doubled", second_val * 2.0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}