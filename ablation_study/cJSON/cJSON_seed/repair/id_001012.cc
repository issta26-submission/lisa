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
//<ID> 1012
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
    cJSON_AddBoolToObject(settings, "enabled", 1);
    cJSON_AddBoolToObject(root, "root_flag", 0);

    // step 2: Configure
    cJSON *policy = cJSON_AddObjectToObject(root, "policy");
    cJSON_AddBoolToObject(policy, "allowed", 0);
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON_AddBoolToObject(root, "settings_present_copy", has_settings);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, settings);
    cJSON_AddItemToObject(policy, "migrated_settings", detached);
    cJSON_bool has_migrated = cJSON_HasObjectItem(policy, "migrated_settings");
    cJSON_AddBoolToObject(root, "migrated_flag", has_migrated);
    cJSON *policy_copy = cJSON_Duplicate(policy, 1);
    char *flat = cJSON_PrintUnformatted(policy_copy);
    cJSON_free(flat);
    cJSON_Delete(policy_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}