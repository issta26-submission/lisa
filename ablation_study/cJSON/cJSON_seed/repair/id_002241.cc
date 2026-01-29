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
//<ID> 2241
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
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *name = cJSON_CreateString("sensor-alpha");
    cJSON_AddItemToObject(config, "name", name);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "version", 2.25);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, n0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, n2);

    // step 3: Operate and Validate
    cJSON *got_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_cfg, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *got_values = cJSON_GetObjectItem(got_cfg, "values");
    cJSON *detached = cJSON_DetachItemFromArray(got_values, 1);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = name_val ? name_val[0] : '\0';
    buffer[2] = detached ? (char)((detached->valueint) & 0xFF) : '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}