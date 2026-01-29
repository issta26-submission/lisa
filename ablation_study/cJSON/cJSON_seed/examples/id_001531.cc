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
//<ID> 1531
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *meta_raw_item = cJSON_AddRawToObject(root, "metadata", "{\"version\":\"1.0\",\"flags\":[true,false]}");

    // step 2: Configure
    char *root_printed = cJSON_Print(root);
    cJSON *fetched_meta = cJSON_GetObjectItemCaseSensitive(root, "metadata");
    cJSON *fetched_config = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *fetched_enabled = cJSON_GetObjectItemCaseSensitive(fetched_config, "enabled");

    // step 3: Operate & Validate
    char *meta_printed = cJSON_Print(fetched_meta);
    cJSON_bool enabled_is_true = cJSON_IsTrue(fetched_enabled);
    (void)enabled_is_true;
    (void)meta_raw_item;

    // step 4: Cleanup
    cJSON_free(root_printed);
    cJSON_free(meta_printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}