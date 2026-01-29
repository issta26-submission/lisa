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
//<ID> 1015
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

    // step 2: Configure
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddBoolToObject(settings, "featureX", 1);
    cJSON_AddBoolToObject(settings, "featureY", 0);

    // step 3: Operate and Validate
    cJSON *feature_item = cJSON_GetObjectItem(settings, "featureX");
    cJSON_bool has_feature = cJSON_HasObjectItem(settings, "featureX");
    cJSON_AddBoolToObject(root, "featureX_present", has_feature);
    cJSON *detached = cJSON_DetachItemViaPointer(settings, feature_item);
    cJSON_AddItemToObject(root, "detached_feature", detached);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}