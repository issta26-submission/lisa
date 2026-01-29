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
//<ID> 813
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNumberToObject(cfg, "threshold", 2.718);
    cJSON_AddStringToObject(cfg, "mode", "fast");

    // step 2: Configure
    cJSON *cfg_lookup = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(cfg_lookup, "threshold");
    double threshold_value = cJSON_GetNumberValue(threshold_item);
    int values_count = cJSON_GetArraySize(values);
    cJSON *detached_cfg = cJSON_DetachItemFromObject(root, "config");

    // step 3: Operate and Validate
    cJSON *restored_cfg = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNumberToObject(restored_cfg, "threshold", threshold_value + (double)values_count);
    cJSON_AddStringToObject(restored_cfg, "recreated", "yes");
    char *snapshot_root = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_Delete(detached_cfg);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}