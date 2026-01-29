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
//<ID> 775
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
    cJSON *sub = cJSON_AddObjectToObject(config, "subconfig");

    // step 2: Configure
    cJSON_AddNumberToObject(sub, "threshold", 3.14159);
    cJSON_AddNumberToObject(sub, "scale", 2.0);
    cJSON_AddStringToObject(config, "mode", "automatic");

    // step 3: Operate and Validate
    cJSON_bool sub_is_object = cJSON_IsObject(sub);
    (void)sub_is_object;
    cJSON *detached_threshold = cJSON_DetachItemFromObject(sub, "threshold");
    cJSON_AddItemToObject(root, "detached_threshold", detached_threshold);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}