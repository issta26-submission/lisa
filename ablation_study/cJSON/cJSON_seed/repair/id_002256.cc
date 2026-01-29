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
//<ID> 2256
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
    cJSON *enabled = cJSON_AddTrueToObject(root, "enabled");
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(config, "count", count);

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *v0 = cJSON_CreateNumber(100.0);
    cJSON *v1 = cJSON_CreateNumber(200.0);
    cJSON_AddItemToArray(values, v0);
    cJSON_AddItemToArray(values, v1);

    // step 3: Operate and Validate
    cJSON *cfg_ref = cJSON_CreateObjectReference(config);
    cJSON_AddItemToObject(root, "config_ref", cfg_ref);
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled);
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON *got_ref = cJSON_GetObjectItem(root, "config_ref");
    cJSON *got_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_cfg, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *got_count = cJSON_GetObjectItem(got_cfg, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    buffer[0] = name_val ? name_val[0] : '\0';
    buffer[1] = (char)((int)count_val % 256);
    buffer[2] = enabled_flag ? '1' : '0';
    buffer[3] = printed_ok ? '1' : '0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}