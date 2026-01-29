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
//<ID> 2225
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
    int ids_arr[3] = {101, 202, 303};
    cJSON *ids = cJSON_CreateIntArray(ids_arr, 3);
    cJSON_AddItemToObject(cfg, "ids", ids);
    cJSON *threshold = cJSON_CreateNumber(9.75);
    cJSON_AddItemToObject(cfg, "threshold", threshold);
    cJSON *name = cJSON_CreateString("device_alpha");
    cJSON_AddItemToObject(cfg, "name", name);

    // step 3: Operate and Validate
    cJSON_bool cfg_is_obj = cJSON_IsObject(cfg);
    (void)cfg_is_obj;
    cJSON *got_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_cfg, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    (void)name_val;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(cfg, "threshold");
    cJSON_DeleteItemFromObjectCaseSensitive(cfg, "name");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}