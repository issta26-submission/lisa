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
//<ID> 1535
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
    cJSON *active = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "active", active);
    cJSON *embedded_raw = cJSON_AddRawToObject(config, "meta", "{\"version\":1,\"valid\":true}");

    // step 2: Configure
    char *printed_root = cJSON_Print(root);
    cJSON *active_item = cJSON_GetObjectItemCaseSensitive(root, "active");
    cJSON *meta_item = cJSON_GetObjectItemCaseSensitive(config, "meta");

    // step 3: Operate & Validate
    cJSON_bool active_is_bool = cJSON_IsBool(active_item);
    cJSON_bool meta_is_raw = cJSON_IsRaw(meta_item);
    const char *meta_value = cJSON_GetStringValue(meta_item);
    (void)active_is_bool;
    (void)meta_is_raw;
    (void)meta_value;
    (void)embedded_raw;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}