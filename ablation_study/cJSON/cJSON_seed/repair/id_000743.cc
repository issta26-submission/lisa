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
//<ID> 743
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

    // step 2: Configure
    cJSON *threshold = cJSON_AddNumberToObject(config, "threshold", 10.5);
    cJSON *scale = cJSON_AddNumberToObject(config, "scale", 2.0);
    double threshold_val = cJSON_GetNumberValue(threshold);
    double scale_val = cJSON_GetNumberValue(scale);

    // step 3: Operate and Validate
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *detached = cJSON_DetachItemFromObject(config, "scale");
    cJSON_AddItemToObject(meta, "moved_scale", detached);
    double moved_scale_val = cJSON_GetNumberValue(detached);
    cJSON_bool meta_is_object = cJSON_IsObject(meta);
    (void)threshold_val;
    (void)scale_val;
    (void)moved_scale_val;
    (void)meta_is_object;

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}