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
//<ID> 1534
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *active_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "active", active_bool);
    cJSON_AddItemToObject(root, "settings", settings);
    const char *raw_meta = "{\"version\":1,\"flags\":{\"beta\":true}}";
    cJSON *raw_item_added = cJSON_AddRawToObject(root, "raw_meta", raw_meta);

    // step 2: Configure
    cJSON_AddNumberToObject(settings, "timeout", 30.0);
    cJSON_AddStringToObject(settings, "mode", "fast");

    // step 3: Operate & Validate
    cJSON *fetched_raw = cJSON_GetObjectItemCaseSensitive(root, "raw_meta");
    cJSON *fetched_active = cJSON_GetObjectItemCaseSensitive(root, "active");
    cJSON *fetched_timeout = cJSON_GetObjectItemCaseSensitive(settings, "timeout");
    const char *raw_value = cJSON_GetStringValue(fetched_raw);
    cJSON_bool active_is_bool = cJSON_IsBool(fetched_active);
    double timeout_value = cJSON_GetNumberValue(fetched_timeout);
    char *printed = cJSON_Print(root);
    (void)raw_item_added;
    (void)raw_value;
    (void)active_is_bool;
    (void)timeout_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}