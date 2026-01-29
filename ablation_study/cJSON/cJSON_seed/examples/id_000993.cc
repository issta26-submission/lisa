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
//<ID> 993
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateIntArray(nums, 4);
    cJSON *config = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device-xyz");
    cJSON *count = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "values", values);
    cJSON_AddItemToObjectCS(root, "config", config);
    cJSON_AddItemToObjectCS(config, "name", name);
    cJSON_AddItemToObjectCS(config, "count", count);

    // step 3: Operate & Validate
    int array_size = cJSON_GetArraySize(values);
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON *got_count = cJSON_GetObjectItem(config, "count");
    double updated = cJSON_SetNumberHelper(got_count, (double)array_size + (double)has_values);
    (void)updated;
    const char *name_str = cJSON_GetStringValue(name);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObjectCS(root, "name_copy", name_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}