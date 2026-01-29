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
//<ID> 748
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *inner_str_item = cJSON_CreateString("inner_value");
    (void)version;

    // step 2: Configure
    cJSON_AddItemToObject(child, "inner", inner_str_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "nullable", null_item);
    cJSON_AddNumberToObject(root, "count", 1.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *got_child = cJSON_GetObjectItem(parsed, "child");
    cJSON *detached_null = cJSON_DetachItemFromObjectCaseSensitive(parsed, "nullable");
    cJSON *inner = cJSON_GetObjectItem(got_child, "inner");
    const char *validated_inner = cJSON_GetStringValue(inner);
    (void)validated_inner;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached_null);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}