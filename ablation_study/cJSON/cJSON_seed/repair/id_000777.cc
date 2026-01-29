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
//<ID> 777
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
    cJSON *settings = cJSON_AddObjectToObject(config, "settings");
    cJSON_AddNumberToObject(config, "threshold", 4.5);
    cJSON_AddNumberToObject(settings, "ratio", 2.0);

    // step 3: Operate and Validate
    cJSON_bool is_obj = cJSON_IsObject(config);
    (void)is_obj;
    cJSON *detached = cJSON_DetachItemFromObject(config, "threshold");
    cJSON_AddItemToObject(meta, "detached_threshold", detached);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}