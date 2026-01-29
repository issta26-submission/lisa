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
//<ID> 2277
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
    cJSON_AddItemToObject(root, "config", config);
    cJSON *enabled_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled_true);
    cJSON *meta_raw = cJSON_CreateRaw("{\"raw\":1}");
    cJSON_AddItemToObject(root, "meta", meta_raw);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "value", 123.0);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *extra = cJSON_CreateString("extra-info");
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_enabled = cJSON_GetObjectItem(got_config, "enabled");
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_raw = cJSON_IsRaw(got_meta);
    cJSON_AddItemToObject(root, "meta_is_raw", cJSON_CreateNumber((double)meta_is_raw));
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)got_enabled;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}