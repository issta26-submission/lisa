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
//<ID> 776
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

    // step 2: Configure
    cJSON_AddNumberToObject(config, "threshold", 4.25);
    cJSON_AddNumberToObject(config, "limit", 10.0);
    cJSON *params = cJSON_AddObjectToObject(config, "params");
    cJSON_AddNumberToObject(params, "retry", 3.0);
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 3: Operate and Validate
    cJSON *config_ref = cJSON_GetObjectItem(root, "config");
    cJSON_bool is_obj = cJSON_IsObject(config_ref);
    cJSON *detached = cJSON_DetachItemFromObject(config_ref, "params");
    cJSON_AddItemToObject(meta, "extracted_params", detached);
    cJSON_AddNumberToObject(meta, "params_was_object", (double)is_obj);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}