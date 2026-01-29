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
//<ID> 1011
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

    // step 2: Configure
    cJSON_AddBoolToObject(config, "featureX", 1);
    cJSON_AddBoolToObject(root, "enabled", 0);

    // step 3: Operate and Validate
    cJSON *feature_item = cJSON_GetObjectItem(config, "featureX");
    cJSON *detached = cJSON_DetachItemViaPointer(config, feature_item);
    cJSON_AddItemToObject(root, "featureX_moved", detached);
    cJSON_bool has_feature = cJSON_HasObjectItem(config, "featureX");
    cJSON_AddBoolToObject(root, "config_has_featureX", has_feature);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    char *pretty = cJSON_Print(root);
    cJSON_free(pretty);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}