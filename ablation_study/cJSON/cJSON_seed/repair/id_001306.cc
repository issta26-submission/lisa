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
//<ID> 1306
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "DeviceX");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "enabled", enabled);
    cJSON *ver = cJSON_CreateString("1.2.3");
    cJSON_AddItemToObjectCS(root, "version", ver);

    // step 2: Configure
    cJSON *status = cJSON_CreateString("idle");
    cJSON_AddItemToObject(root, "status", status);
    char *tmp_val = cJSON_SetValuestring(status, "running");

    // step 3: Operate and Validate
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled);
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled);
    char *out = cJSON_PrintUnformatted(root);
    char *name_val = cJSON_GetStringValue(name_item);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = name_val ? name_val[0] : 0;
    ((char *)scratch)[2] = (char)(enabled_is_bool + enabled_is_true);
    (void)tmp_val;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}