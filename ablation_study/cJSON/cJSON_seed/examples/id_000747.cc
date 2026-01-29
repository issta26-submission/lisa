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
//<ID> 747
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw = "{\"config\":{\"enabled\":true,\"threshold\":2.5}}";
    cJSON *parsed = cJSON_Parse(raw);
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON *config_item = cJSON_GetObjectItem(parsed, "config");
    cJSON *detached_config = cJSON_DetachItemFromObjectCaseSensitive(parsed, "config");
    cJSON_AddItemToObject(root, "migrated_config", detached_config);
    cJSON_AddItemToObject(root, "note", null_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *reparsed = cJSON_Parse(buffer);
    cJSON *found = cJSON_GetObjectItem(reparsed, "migrated_config");
    cJSON *threshold = cJSON_GetObjectItem(found, "threshold");
    double thr_val = cJSON_GetNumberValue(threshold);
    (void)thr_val;
    (void)config_item;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(reparsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}