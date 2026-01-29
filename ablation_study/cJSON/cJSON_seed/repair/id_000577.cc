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
//<ID> 577
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(info, "name", name);
    cJSON *count = cJSON_CreateNumber(12.75);
    cJSON_AddItemToObject(info, "count", count);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(info, "enabled", flag);

    // step 2: Configure
    char *old_name_snapshot = cJSON_SetValuestring(name, "renamed_name");
    double count_val = cJSON_GetNumberValue(count);
    cJSON_bool was_false = cJSON_IsFalse(flag);
    cJSON_AddNumberToObject(root, "enabled_numeric", (double)was_false);
    cJSON *derived = cJSON_CreateNumber(count_val * 1.5);
    cJSON_AddItemToObject(info, "derived", derived);

    // step 3: Operate and Validate
    char *current_name = cJSON_GetStringValue(name);
    cJSON_AddStringToObject(root, "final_name", current_name);
    cJSON_AddStringToObject(root, "old_name_snapshot", old_name_snapshot);
    double derived_val = cJSON_GetNumberValue(derived);
    cJSON_AddNumberToObject(root, "validated_sum", derived_val + count_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}