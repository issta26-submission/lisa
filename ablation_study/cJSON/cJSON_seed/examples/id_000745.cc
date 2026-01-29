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
//<ID> 745
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
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON *str = cJSON_CreateString("example");
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "count", num);
    cJSON_AddItemToObject(config, "greet", str);
    cJSON_AddItemToObject(config, "optional", nul);
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));

    // step 2: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);

    // step 3: Validate
    cJSON *found = cJSON_GetObjectItem(parsed, "config");
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(parsed, "config");
    cJSON *count_item = cJSON_GetObjectItem(detached, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *opt_item = cJSON_GetObjectItem(detached, "optional");
    cJSON_bool opt_is_null = cJSON_IsNull(opt_item);
    (void)found;
    (void)count_val;
    (void)opt_is_null;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}