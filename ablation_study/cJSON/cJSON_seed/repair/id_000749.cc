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
//<ID> 749
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
    cJSON_AddItemToObject(root, "config", config);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *threshold = cJSON_AddNumberToObject(config, "threshold", 3.5);
    cJSON *limit = cJSON_AddNumberToObject(config, "limit", 10.0);
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate and Validate
    double thr_val = cJSON_GetNumberValue(threshold);
    double lim_val = cJSON_GetNumberValue(limit);
    double product = thr_val * lim_val;
    cJSON_AddNumberToObject(meta, "computed_product", product);
    cJSON_bool config_is_obj = cJSON_IsObject(config);
    cJSON_AddNumberToObject(root, "config_is_object", (double)config_is_obj);
    cJSON *detached_limit = cJSON_DetachItemFromObject(config, "limit");
    cJSON_AddItemToObject(root, "detached_limit", detached_limit);
    double detached_val = cJSON_GetNumberValue(detached_limit);
    cJSON_AddNumberToObject(meta, "detached_value_copy", detached_val);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}