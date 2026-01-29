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
//<ID> 1013
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
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *sub = cJSON_AddObjectToObject(config, "sub");
    cJSON_AddBoolToObject(sub, "inner_flag", 1);
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON_AddBoolToObject(root, "has_config", has_config);

    // step 3: Operate and Validate
    cJSON *detached_sub = cJSON_DetachItemViaPointer(config, sub);
    cJSON_AddItemToObject(root, "detached_sub", detached_sub);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    char *pretty = cJSON_Print(root);
    cJSON_free(pretty);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}