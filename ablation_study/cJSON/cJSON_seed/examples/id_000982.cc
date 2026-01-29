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
//<ID> 982
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
    cJSON *device = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(device, "name", cJSON_CreateString("thermo-9"));
    cJSON_AddNumberToObject(device, "id", 42.0);
    cJSON *enabled_flag = cJSON_AddFalseToObject(device, "enabled");
    cJSON_AddItemToObject(config, "device", device);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *override_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "override", override_flag);

    // step 3: Operate & Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_device = cJSON_GetObjectItem(got_config, "device");
    cJSON *got_enabled = cJSON_GetObjectItem(got_device, "enabled");
    cJSON_bool was_disabled = cJSON_IsFalse(got_enabled);
    cJSON_AddNumberToObject(root, "was_disabled", (double)was_disabled);
    cJSON *got_override = cJSON_GetObjectItem(root, "override");
    cJSON_bool override_is_false = cJSON_IsFalse(got_override);
    cJSON_AddNumberToObject(root, "override_was_false", (double)override_is_false);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}