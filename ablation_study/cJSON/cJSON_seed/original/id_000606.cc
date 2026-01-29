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
//<ID> 606
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
    cJSON *mode = cJSON_CreateString("auto");
    cJSON *threshold = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(cfg, "mode", mode);
    cJSON_AddItemToObject(cfg, "threshold", threshold);

    // step 2: Configure
    cJSON *cfg_dup_deep = cJSON_Duplicate(cfg, 1);
    cJSON *cfg_dup_shallow = cJSON_Duplicate(cfg, 0);
    cJSON_AddItemToObject(root, "config_copy", cfg_dup_deep);
    cJSON_Delete(cfg_dup_shallow);

    // step 3: Operate
    cJSON *copied = cJSON_GetObjectItem(root, "config_copy");
    cJSON *mode_item = cJSON_GetObjectItem(copied, "mode");
    const char *mode_str = cJSON_GetStringValue(mode_item);
    cJSON *override = cJSON_CreateString("manual");
    cJSON_AddItemToObject(copied, "mode_override", override);

    // step 4: Validate & Cleanup
    cJSON *verify = cJSON_GetObjectItem(root, "config_copy");
    cJSON *mode_override = cJSON_GetObjectItem(verify, "mode_override");
    const char *override_str = cJSON_GetStringValue(mode_override);
    (void)mode_str;
    (void)override_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}