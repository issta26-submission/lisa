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
//<ID> 555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_AddObjectToObject(root, "stats");
    cJSON *base_num = cJSON_AddNumberToObject(stats, "base", 42.0);
    cJSON *mutable_num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(mutable_num, 7.5);
    cJSON_AddItemToObject(stats, "mutable", mutable_num);

    // step 2: Configure
    cJSON *stats_copy = cJSON_Duplicate(stats, 1);
    double base_val = cJSON_GetNumberValue(base_num);
    double mut_val = cJSON_GetNumberValue(mutable_num);
    cJSON *sum_item = cJSON_AddNumberToObject(root, "sum", base_val + mut_val);

    // step 3: Operate and Validate
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *stats_dup = cJSON_GetObjectItem(root_dup, "stats");
    cJSON *dup_base = cJSON_GetObjectItem(stats_dup, "base");
    double dup_base_val = cJSON_GetNumberValue(dup_base);
    cJSON *double_base = cJSON_CreateNumber(dup_base_val * 2.0);
    cJSON_AddItemToObject(root_dup, "double_base", double_base);
    cJSON *sum_dup = cJSON_GetObjectItem(root_dup, "sum");
    cJSON_SetNumberHelper(sum_dup, dup_base_val + cJSON_GetNumberValue(cJSON_GetObjectItem(stats_dup, "mutable")));

    // step 4: Cleanup
    cJSON_Delete(root_dup);
    cJSON_Delete(stats_copy);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}