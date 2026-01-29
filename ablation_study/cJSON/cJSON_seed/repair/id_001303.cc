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
//<ID> 1303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "DeviceX");
    cJSON *version_item = cJSON_AddStringToObject(meta, "version", "1.2.3");
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "enabled", flag);
    cJSON *desc_item = cJSON_AddStringToObject(root, "desc", "Test unit");

    // step 2: Configure
    cJSON_bool enabled_is_bool = cJSON_IsBool(flag);
    cJSON_bool enabled_is_true = cJSON_IsTrue(flag);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + enabled_is_bool + enabled_is_true);
    ((char *)scratch)[1] = out[0];
    const char *name_val = cJSON_GetStringValue(name_item);
    ((char *)scratch)[2] = name_val[0];
    ((char *)scratch)[3] = version_item->valuestring ? version_item->valuestring[0] : 0;
    ((char *)scratch)[4] = desc_item->valuestring ? desc_item->valuestring[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}