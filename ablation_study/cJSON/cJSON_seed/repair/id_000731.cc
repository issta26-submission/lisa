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
//<ID> 731
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
    cJSON *threshold = cJSON_CreateNumber(10.0);
    cJSON *limit = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON_AddItemToObject(config, "limit", limit);

    // step 2: Configure
    cJSON *config_backup = cJSON_Duplicate(config, 1);
    cJSON *orig_threshold = cJSON_GetObjectItem(config, "threshold");
    cJSON *new_threshold = cJSON_CreateNumber(20.5);
    cJSON_ReplaceItemViaPointer(config, orig_threshold, new_threshold);

    // step 3: Operate and Validate
    cJSON *th_ref = cJSON_GetObjectItem(config, "threshold");
    cJSON *limit_ref = cJSON_GetObjectItem(config, "limit");
    double sum_val = cJSON_GetNumberValue(th_ref) + cJSON_GetNumberValue(limit_ref);
    cJSON *sum_item = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(config_backup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}