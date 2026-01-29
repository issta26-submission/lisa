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
//<ID> 1016
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
    cJSON_AddBoolToObject(settings, "active", 1);
    cJSON_AddBoolToObject(root, "debug", 0);

    // step 2: Configure
    cJSON *active_item = cJSON_GetObjectItem(settings, "active");
    cJSON *detached_active = cJSON_DetachItemViaPointer(settings, active_item);
    cJSON_AddItemToObject(root, "active_moved", detached_active);

    // step 3: Operate and Validate
    cJSON_bool present = cJSON_HasObjectItem(root, "active_moved");
    cJSON_AddBoolToObject(root, "active_present", present);
    cJSON *moved = cJSON_GetObjectItem(root, "active_moved");
    cJSON *dup = cJSON_Duplicate(moved, 1);
    cJSON_AddItemToObject(settings, "active_copy", dup);
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_free(json_str);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}