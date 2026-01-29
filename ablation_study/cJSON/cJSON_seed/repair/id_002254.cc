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
//<ID> 2254
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
    cJSON *enabled = cJSON_AddTrueToObject(config, "enabled");
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(config, "name", name);

    // step 2: Configure
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "count", count);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *v0 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(values, v0);
    cJSON *v1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, v1);
    cJSON *enabled_ref = cJSON_CreateObjectReference(enabled);
    cJSON_AddItemToObject(root, "enabled_ref", enabled_ref);

    // step 3: Operate and Validate
    cJSON *got_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *got_enabled = cJSON_GetObjectItem(got_cfg, "enabled");
    int is_true_enabled = (int)cJSON_IsTrue(got_enabled);
    cJSON *got_ref = cJSON_GetObjectItem(root, "enabled_ref");
    int is_true_ref = (int)cJSON_IsTrue(got_ref);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 0);
    (void)printed_ok;
    buffer[0] = (char)('0' + (is_true_enabled ? 1 : 0));
    buffer[1] = (char)('0' + (is_true_ref ? 1 : 0));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}