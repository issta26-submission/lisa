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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *name_item = cJSON_CreateString("device1");
    cJSON_AddItemToObject(cfg, "name", name_item);
    cJSON *enabled_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(cfg, "enabled", enabled_item);

    // step 2: Configure
    cJSON *retrieved_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *retrieved_name = cJSON_GetObjectItem(retrieved_cfg, "name");
    cJSON *retrieved_enabled = cJSON_GetObjectItem(retrieved_cfg, "enabled");

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(retrieved_name);
    (void)name_val;
    int is_enabled = (int)cJSON_IsTrue(retrieved_enabled);
    int has_name = (int)(retrieved_name != (cJSON *)0);
    cJSON_AddNumberToObject(root, "validation", (double)(is_enabled + has_name));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}