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
//<ID> 773
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
    cJSON_AddNumberToObject(config, "max_retries", 5.0);
    cJSON_AddStringToObject(meta, "note", "migrating config");

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "config");
    cJSON_bool is_obj = cJSON_IsObject(detached);
    cJSON_AddNumberToObject(meta, "detached_is_object", (double)is_obj);
    cJSON_AddItemToObject(meta, "migrated_config", detached);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}