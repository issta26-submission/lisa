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
//<ID> 778
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
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 2: Configure
    cJSON *service = cJSON_AddObjectToObject(config, "service");
    cJSON_AddNumberToObject(service, "version", 3.14);
    cJSON_AddNumberToObject(config, "timeout_ms", 2500.0);
    cJSON_AddNumberToObject(metrics, "initial_count", 0.0);

    // step 3: Operate and Validate
    cJSON_bool service_is_obj = cJSON_IsObject(service);
    cJSON_AddNumberToObject(metrics, "service_is_object", (double)service_is_obj);
    cJSON *detached_service = cJSON_DetachItemFromObject(config, "service");
    cJSON_AddItemToObject(root, "detached_service", detached_service);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}