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
//<ID> 2247
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
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(cfg, "name", name);
    cJSON *ver = cJSON_AddNumberToObject(cfg, "version", 1.2);

    // step 2: Configure
    cJSON *vals = cJSON_CreateArray();
    cJSON_AddItemToObject(cfg, "values", vals);
    cJSON_AddItemToArray(vals, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(vals, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(vals, cJSON_CreateNumber(30.0));

    // step 3: Operate and Validate
    cJSON *got_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_cfg, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *got_vals = cJSON_GetObjectItem(got_cfg, "values");
    cJSON *detached = cJSON_DetachItemFromArray(got_vals, 1);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = name_val ? name_val[0] : '\0';
    buffer[3] = ver ? (char)((int)ver->valuedouble % 256) : '\0';
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(detached);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}