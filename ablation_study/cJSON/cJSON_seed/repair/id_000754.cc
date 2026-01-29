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
//<ID> 754
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *raw_cfg = cJSON_CreateRaw("{\"enabled\":true}");
    cJSON_AddItemToObject(config, "raw_config", raw_cfg);
    cJSON *opt_null = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "optional", opt_null);
    cJSON *params = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "params", params);
    cJSON *threshold = cJSON_AddNumberToObject(params, "threshold", 2.5);

    // step 3: Operate and Validate
    cJSON *config_ref = cJSON_GetObjectItem(root, "config");
    cJSON *raw_ref = cJSON_GetObjectItem(config_ref, "raw_config");
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *opt_ref = cJSON_GetObjectItem(meta_ref, "optional");
    cJSON *raw_dup = cJSON_Duplicate(raw_ref, 1);
    cJSON_AddItemToObject(root, "raw_copy", raw_dup);
    cJSON *params_ref = cJSON_GetObjectItem(config_ref, "params");
    cJSON *threshold_ref = cJSON_GetObjectItem(params_ref, "threshold");
    double thr_val = cJSON_GetNumberValue(threshold_ref);
    cJSON_AddNumberToObject(meta_ref, "thr_snapshot", thr_val);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}