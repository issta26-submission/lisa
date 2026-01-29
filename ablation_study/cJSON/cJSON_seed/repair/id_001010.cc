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
//<ID> 1010
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
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddBoolToObject(settings, "enabled", 1);
    cJSON *global_item = cJSON_AddBoolToObject(root, "global", 0);

    // step 3: Operate
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON_AddBoolToObject(meta, "settings_present", has_settings);
    cJSON *got_global = cJSON_GetObjectItem(root, "global");
    cJSON *detached_global = cJSON_DetachItemViaPointer(root, got_global);
    cJSON_bool relocate_ok = cJSON_AddItemToObject(meta, "global_relocated", detached_global);

    // step 4: Validate and Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    (void)enabled_item;
    (void)global_item;
    (void)relocate_ok;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}