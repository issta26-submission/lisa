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
//<ID> 556
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *threshold = cJSON_AddNumberToObject(config, "threshold", 3.14);
    cJSON *base_num = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "base", base_num);

    // step 2: Configure
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);
    double prev_threshold = cJSON_SetNumberHelper(threshold, 4.5);

    // step 3: Operate and Validate
    double thr_val = cJSON_GetNumberValue(threshold);
    double base_val = cJSON_GetNumberValue(base_num);
    cJSON *sum = cJSON_CreateNumber(base_val + thr_val + prev_threshold);
    cJSON_AddItemToObject(root, "sum", sum);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *dup_config = cJSON_GetObjectItem(root_dup, "config");
    cJSON *dup_threshold = cJSON_GetObjectItem(dup_config, "threshold");
    double dup_thr_val = cJSON_GetNumberValue(dup_threshold);
    cJSON *dup_sum = cJSON_CreateNumber(dup_thr_val + base_val);
    cJSON_AddItemToObject(root_dup, "dup_sum", dup_sum);
    char *out = cJSON_PrintUnformatted(root_dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}