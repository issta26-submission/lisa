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
//<ID> 49
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
    cJSON *enabled = cJSON_CreateTrue();
    cJSON *username = cJSON_CreateString("tester");

    // step 2: Configure
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON_AddItemToObject(config, "username", username);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "env", "dev");

    // step 3: Operate and Validate
    cJSON *cfg_from_root = cJSON_GetObjectItem(root, "config");
    cJSON *enabled_chk = cJSON_GetObjectItem(cfg_from_root, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_chk);
    cJSON *name_chk = cJSON_GetObjectItem(cfg_from_root, "username");
    const char *name_val = cJSON_GetStringValue(name_chk);
    int validation_value = (int)is_enabled + (int)name_val[0];
    cJSON_AddNumberToObject(root, "validation", (double)validation_value);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}