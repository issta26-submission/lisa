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
//<ID> 774
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *config = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    cJSON *params = cJSON_AddObjectToObject(config, "params");
    cJSON_AddNumberToObject(config, "threshold", 4.5);
    cJSON_AddNumberToObject(params, "scale", 2.0);
    cJSON_AddNumberToObject(params, "offset", 1.25);

    // step 3: Operate and Validate
    cJSON_bool params_is_obj = cJSON_IsObject(params);
    cJSON *detached = cJSON_DetachItemFromObject(config, "threshold");
    double detached_val = cJSON_GetNumberValue(detached);
    cJSON_AddItemToObject(root, "detached_threshold", detached);
    cJSON *scale_item = cJSON_GetObjectItem(params, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    double recomputed = detached_val * scale;
    cJSON_AddNumberToObject(meta, "recomputed", recomputed);
    cJSON_AddNumberToObject(meta, "params_is_object", (double)params_is_obj);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}