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
//<ID> 1017
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddBoolToObject(settings, "featureX", 0);

    // step 2: Configure
    cJSON *flags = cJSON_AddObjectToObject(settings, "flags");
    cJSON_AddBoolToObject(flags, "beta", 1);
    cJSON_AddBoolToObject(flags, "legacy", 0);
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON_AddBoolToObject(root, "has_settings", has_settings);

    // step 3: Operate
    cJSON *settings_ptr = cJSON_GetObjectItem(root, "settings");
    cJSON *detached = cJSON_DetachItemViaPointer(root, settings_ptr);
    cJSON_AddItemToObject(root, "restored_settings", detached);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}