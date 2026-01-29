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
//<ID> 1018
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
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *options = cJSON_AddObjectToObject(settings, "options");
    cJSON_AddBoolToObject(options, "opt1", 1);
    cJSON_AddBoolToObject(options, "opt2", 0);

    // step 3: Operate and Validate
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON_AddBoolToObject(root, "has_settings", has_settings);
    cJSON *options_ptr = cJSON_GetObjectItem(settings, "options");
    cJSON *detached = cJSON_DetachItemViaPointer(settings, options_ptr);
    cJSON_AddItemToObject(root, "moved_options", detached);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}