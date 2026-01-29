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
//<ID> 44
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
    cJSON *enabled_flag = cJSON_CreateTrue();
    cJSON *device_name = cJSON_CreateString("device01");

    // step 2: Configure
    cJSON_AddItemToObject(config, "enabled", enabled_flag);
    cJSON_AddItemToObject(config, "name", device_name);
    cJSON *meta = cJSON_AddObjectToObject(config, "meta");
    cJSON_AddStringToObject(meta, "version", "v1.2");

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_enabled = cJSON_GetObjectItem(got_config, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(got_enabled);
    cJSON *got_name = cJSON_GetObjectItem(got_config, "name");
    const char *name_val = cJSON_GetStringValue(got_name);
    cJSON *summary = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "summary", summary);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}