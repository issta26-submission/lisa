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
//<ID> 1537
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
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(settings, "enabled", enabled);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddRawToObject(root, "raw_meta", "{\"version\":1,\"name\":\"test\"}");

    // step 2: Configure & Operate
    char *printed = cJSON_Print(root);
    cJSON *retrieved_enabled = cJSON_GetObjectItemCaseSensitive(settings, "enabled");
    cJSON *retrieved_raw = cJSON_GetObjectItemCaseSensitive(root, "raw_meta");

    // step 3: Validate
    cJSON_bool enabled_is_bool = cJSON_IsBool(retrieved_enabled);
    const char *raw_value = cJSON_GetStringValue(retrieved_raw);
    (void)enabled_is_bool;
    (void)raw_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}