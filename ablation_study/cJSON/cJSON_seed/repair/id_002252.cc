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
//<ID> 2252
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
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "child", child);
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(child, "name", name);
    cJSON *enabled = cJSON_AddTrueToObject(config, "enabled");

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_enabled = cJSON_GetObjectItem(got_config, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(got_enabled);
    cJSON *got_child = cJSON_GetObjectItem(got_config, "child");
    cJSON *got_name = cJSON_GetObjectItem(got_child, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 0);
    buffer[0] = name_val ? name_val[0] : '\0';
    buffer[1] = (char)(is_enabled ? 1 : 0);
    buffer[2] = buffer[0];
    buffer[3] = buffer[1];

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}