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
//<ID> 2242
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
    cJSON *name = cJSON_CreateString("device_alpha");
    cJSON_AddItemToObject(cfg, "name", name);
    cJSON *vals = cJSON_CreateArray();
    cJSON_AddItemToObject(cfg, "values", vals);
    cJSON *v0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(vals, v0);
    cJSON *v1 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(vals, v1);
    cJSON *v2 = cJSON_CreateNumber(30.25);
    cJSON_AddItemToArray(vals, v2);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(vals, 1);
    double detached_val = cJSON_GetNumberValue(detached);
    cJSON *detached_node = cJSON_AddNumberToObject(root, "detached_value", detached_val);

    // step 3: Operate and Validate
    cJSON *got_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_cfg, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 80;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = name_val ? name_val[0] : '\0';
    buffer[3] = (char)((int)detached_val % 256);
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(detached); /* detached was removed from array and must be deleted */
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}