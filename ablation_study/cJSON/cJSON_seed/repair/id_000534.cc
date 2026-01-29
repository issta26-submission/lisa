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
//<ID> 534
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    const cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *scaled = cJSON_CreateNumber(count_val * 1.5);
    cJSON_AddItemToObject(cfg, "scaled_count", scaled);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(cfg, "flag", flag);
    cJSON *cfg_copy = cJSON_Duplicate(cfg, 1);
    cJSON_AddItemToObject(root, "config_copy", cfg_copy);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    const cJSON *copied_item = cJSON_GetObjectItem(root, "config_copy");
    const cJSON *copied_scaled = cJSON_GetObjectItem(copied_item, "scaled_count");
    double copied_value = cJSON_GetNumberValue(copied_scaled);
    (void)copied_value;

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}