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
//<ID> 772
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "threshold", 4.25);
    cJSON_AddNumberToObject(config, "timeout", 30.0);
    cJSON *params = cJSON_AddObjectToObject(config, "params");
    cJSON_AddStringToObject(params, "mode", "auto");
    cJSON_AddNumberToObject(params, "retry", 3.0);

    // step 3: Operate and Validate
    cJSON_AddBoolToObject(root, "config_is_object", cJSON_IsObject(config));
    cJSON *detached_threshold = cJSON_DetachItemFromObject(config, "threshold");
    cJSON_AddItemToObject(root, "detached_threshold", detached_threshold);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}