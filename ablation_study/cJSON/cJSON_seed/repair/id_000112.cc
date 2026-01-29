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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "nums", nums);
    cJSON *n0 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(nums, n0);
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(nums, n1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(meta, "name", name);

    // step 2: Configure
    cJSON *dup_nums = cJSON_Duplicate(nums, 1);
    cJSON_AddItemToObject(root, "backup", dup_nums);
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate and Validate
    cJSON *got_nums = cJSON_GetObjectItem(root, "nums");
    cJSON *item_idx1 = cJSON_GetArrayItem(got_nums, 1);
    double val_idx1 = cJSON_GetNumberValue(item_idx1);
    cJSON *got_backup = cJSON_GetObjectItem(root, "backup");
    cJSON *backup_item0 = cJSON_GetArrayItem(got_backup, 0);
    double val_backup0 = cJSON_GetNumberValue(backup_item0);
    double combined = val_idx1 + val_backup0;
    cJSON_AddNumberToObject(root, "combined", combined);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}