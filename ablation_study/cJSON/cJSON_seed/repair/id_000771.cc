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
//<ID> 771
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
    cJSON *limits = cJSON_AddObjectToObject(config, "limits");
    cJSON_AddNumberToObject(limits, "min", 1.0);
    cJSON_AddNumberToObject(limits, "max", 10.0);
    cJSON_AddNumberToObject(meta, "config_is_object", (double)cJSON_IsObject(config));

    // step 3: Operate and Validate
    cJSON *detached_limits = cJSON_DetachItemFromObject(config, "limits");
    cJSON_AddItemToObject(meta, "detached_limits", detached_limits);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}