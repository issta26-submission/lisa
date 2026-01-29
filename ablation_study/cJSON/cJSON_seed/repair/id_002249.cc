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
//<ID> 2249
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
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *count = cJSON_AddNumberToObject(config, "count", 3.0);

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *v0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, v0);
    cJSON *v1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, v1);

    // step 3: Operate and Validate
    cJSON *got_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_cfg, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *got_count = cJSON_GetObjectItem(got_cfg, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    cJSON *detached = cJSON_DetachItemFromArray(values, 1);
    cJSON_AddItemToObject(root, "detached_value", detached);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = name_val ? name_val[0] : '\0';
    buffer[2] = (char)((int)count_val % 256);
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}