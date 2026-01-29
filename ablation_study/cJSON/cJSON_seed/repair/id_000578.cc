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
//<ID> 578
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize and Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *mode = cJSON_CreateString("off");
    cJSON_AddItemToObject(config, "mode", mode);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *threshold = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(config, "threshold", threshold);

    // step 2: Operate
    char *orig_mode = cJSON_GetStringValue(mode);
    char *set_ret = cJSON_SetValuestring(mode, "on");
    cJSON *fetched_mode = cJSON_GetObjectItem(config, "mode");
    char *mode_value = cJSON_GetStringValue(fetched_mode);
    double thresh = cJSON_GetNumberValue(threshold);
    cJSON_bool was_false = cJSON_IsFalse(enabled);
    double adjusted = thresh + (double)was_false;
    cJSON_AddNumberToObject(root, "adjusted_threshold", adjusted);

    // step 3: Validate
    cJSON *adj_item = cJSON_GetObjectItem(root, "adjusted_threshold");
    double adj_val = cJSON_GetNumberValue(adj_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}