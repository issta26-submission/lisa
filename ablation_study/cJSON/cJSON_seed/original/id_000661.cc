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
//<ID> 661
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
    cJSON *inner = cJSON_CreateObject();
    cJSON *enabled_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "inner", inner);
    cJSON_AddItemToObject(config, "enabled", enabled_true);
    cJSON_AddNumberToObject(inner, "value", 123.0);
    cJSON_AddStringToObject(config, "name", "example");

    // step 2: Configure
    cJSON *inner_copy = cJSON_Duplicate(inner, 1);
    cJSON_AddItemToObject(root, "inner_copy", inner_copy);

    // step 3: Operate
    cJSON *detached_enabled = cJSON_DetachItemFromObject(config, "enabled");
    cJSON_AddItemToObject(root, "enabled_moved", detached_enabled);
    cJSON *detached_inner = cJSON_DetachItemFromObjectCaseSensitive(root, "inner");
    cJSON_AddItemToObject(config, "inner_taken", detached_inner);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}