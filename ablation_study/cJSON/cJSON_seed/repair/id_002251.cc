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
//<ID> 2251
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
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "count", count);
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *config_ref = cJSON_CreateObjectReference(config);
    cJSON_AddItemToObject(root, "config_ref", config_ref);

    // step 2: Configure
    cJSON *got_cfg_via_ref = cJSON_GetObjectItem(root, "config_ref");
    (void)got_cfg_via_ref; // keep reference variable to show meaningful flow

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_enabled = cJSON_GetObjectItem(got_config, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(got_enabled);
    cJSON *got_count = cJSON_GetObjectItem(got_config, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)((int)is_enabled + '0');
    buffer[1] = (char)((int)count_val % 256);
    buffer[2] = (char)(printed ? 'P' : 'F');

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}