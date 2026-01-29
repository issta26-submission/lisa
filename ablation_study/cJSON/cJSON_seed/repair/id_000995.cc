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
//<ID> 995
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "test_project");
    cJSON_AddNullToObject(root, "optional");
    cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON_AddNumberToObject(config, "threshold", 2.71828);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 3: Operate and Validate
    cJSON *cfg_item = cJSON_GetObjectItem(root, "config");
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(cfg_item, "enabled");
    cJSON *optional_item = cJSON_GetObjectItem(root, "optional");
    cJSON_bool optional_is_null = cJSON_IsNull(optional_item);
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled_item);
    (void)printed;
    (void)optional_is_null;
    (void)enabled_is_true;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}