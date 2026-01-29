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
//<ID> 650
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
    cJSON_AddItemToObjectCS(root, "config", config);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(config, "enabled", enabled);
    cJSON *threshold = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObjectCS(config, "threshold", threshold);
    cJSON_AddNumberToObject(root, "max_limit", 100.0);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "scale", 2.5);
    cJSON_bool has_enabled = cJSON_HasObjectItem(config, "enabled");
    cJSON_AddNumberToObject(root, "has_enabled", (double)has_enabled);
    cJSON_AddNumberToObject(root, "multiplier", 1.5);

    // step 3: Operate and Validate
    cJSON *thr_item = cJSON_GetObjectItem(config, "threshold");
    double thr_value = cJSON_GetNumberValue(thr_item);
    cJSON_AddNumberToObject(root, "threshold_readback", thr_value);
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObjectCS(root, "config_copy", config_copy);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}