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
//<ID> 749
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON *age_item = cJSON_CreateNumber(30.0);

    // step 2: Configure
    cJSON_AddItemToObject(child, "name", name_item);
    cJSON_AddItemToObject(child, "age", age_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "nullable", null_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *detached_child = cJSON_DetachItemFromObjectCaseSensitive(parsed, "child");
    cJSON *detached_name = cJSON_GetObjectItem(detached_child, "name");
    cJSON *parsed_nullable = cJSON_GetObjectItem(parsed, "nullable");
    const char *extracted_name = cJSON_GetStringValue(detached_name);
    (void)extracted_name;
    (void)parsed_nullable;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached_child);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}