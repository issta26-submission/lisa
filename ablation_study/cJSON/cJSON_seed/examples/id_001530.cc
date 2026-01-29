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
//<ID> 1530
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *active = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "active", active);
    cJSON_AddRawToObject(root, "meta", "{\"version\":1,\"name\":\"example\"}");

    // step 2: Configure
    char *printed = cJSON_Print(root);

    // step 3: Operate & Validate
    cJSON *active_item = cJSON_GetObjectItemCaseSensitive(root, "active");
    cJSON_bool active_is_true = cJSON_IsTrue(active_item);
    cJSON *meta_item = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON_bool meta_is_raw = cJSON_IsRaw(meta_item);
    const char *meta_value = cJSON_GetStringValue(meta_item);
    cJSON *meta_copy = cJSON_CreateString(meta_value ? meta_value : "");
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);
    (void)active_is_true;
    (void)meta_is_raw;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}