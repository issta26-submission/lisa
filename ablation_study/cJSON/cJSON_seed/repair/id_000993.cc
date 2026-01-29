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
//<ID> 993
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
    cJSON_AddNullToObject(config, "optional");
    cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON_AddBoolToObject(meta, "verified", 0);

    // step 3: Operate and Validate
    cJSON *opt_item = cJSON_GetObjectItem(config, "optional");
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(config, "enabled");
    cJSON_bool opt_is_null = cJSON_IsNull(opt_item);
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_prealloc = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    (void)opt_is_null;
    (void)enabled_is_bool;
    (void)printed_prealloc;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}