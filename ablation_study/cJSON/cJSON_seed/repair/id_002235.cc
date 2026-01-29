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
//<ID> 2235
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
    cJSON *mode = cJSON_CreateString("fast");
    cJSON_AddItemToObject(cfg, "mode", mode);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(cfg, "count", count);
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    cJSON *vals = cJSON_CreateArray();
    cJSON_AddItemToObject(cfg, "values", vals);
    cJSON *v0 = cJSON_CreateNumber(1.0);
    cJSON *v1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(vals, v0);
    cJSON_AddItemToArray(vals, v1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *tag = cJSON_CreateString("sample");
    cJSON_AddItemToObject(meta, "tag", tag);

    // step 3: Operate and Validate
    cJSON *got_mode = cJSON_GetObjectItem(cfg, "mode");
    char *mode_val = cJSON_GetStringValue(got_mode);
    cJSON *got_count = cJSON_GetObjectItem(cfg, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    (void)mode_val;
    (void)count_val;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(cfg, "mode");
    cJSON_DeleteItemFromObject(root, "config");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}