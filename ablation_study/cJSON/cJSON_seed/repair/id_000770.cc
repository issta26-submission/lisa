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
//<ID> 770
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *params = cJSON_AddObjectToObject(config, "params");

    // step 2: Configure
    cJSON_AddNumberToObject(config, "threshold", 4.25);
    cJSON_AddNumberToObject(params, "timeout_ms", 3000.0);
    cJSON_AddNumberToObject(params, "retries", 3.0);

    // step 3: Operate and Validate
    cJSON *detached_params = cJSON_DetachItemFromObject(config, "params");
    cJSON_bool was_object = cJSON_IsObject(detached_params);
    cJSON_AddNumberToObject(detached_params, "was_object_flag", was_object ? 1.0 : 0.0);
    cJSON_AddItemToObject(meta, "saved_params", detached_params);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}