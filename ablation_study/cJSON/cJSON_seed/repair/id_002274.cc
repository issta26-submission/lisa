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
//<ID> 2274
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
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *raw = cJSON_CreateRaw("{\"x\":1}");
    cJSON_AddItemToObject(root, "rawdata", raw);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "value", cJSON_CreateNumber(42.0));
    cJSON_AddItemToObject(root, "child", child);
    const int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_enabled = cJSON_GetObjectItem(got_config, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(got_enabled);
    cJSON *replicated = cJSON_CreateBool(enabled_flag);
    cJSON_AddItemToObject(root, "replicated_enabled", replicated);
    cJSON *got_raw = cJSON_GetObjectItem(root, "rawdata");
    cJSON_bool raw_flag = cJSON_IsRaw(got_raw);
    cJSON *raw_flag_node = cJSON_CreateBool(raw_flag);
    cJSON_AddItemToObject(root, "raw_is_raw", raw_flag_node);
    (void)printed;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}