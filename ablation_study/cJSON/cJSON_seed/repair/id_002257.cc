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
//<ID> 2257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(cfg, "name", name);
    cJSON *enabled = cJSON_AddTrueToObject(cfg, "enabled");
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(cfg, "count", count);

    // step 3: Operate and Validate
    cJSON *got_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *got_enabled = cJSON_GetObjectItem(got_cfg, "enabled");
    cJSON_bool is_true = cJSON_IsTrue(got_enabled);
    cJSON *cfg_ref = cJSON_CreateObjectReference(got_cfg);
    cJSON_AddItemToObject(root, "config_ref", cfg_ref);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    char *name_val = cJSON_GetStringValue(name);
    buffer[0] = (char)('0' + (int)is_true);
    buffer[1] = (char)('0' + (int)printed);
    buffer[2] = name_val ? name_val[0] : '\0';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}