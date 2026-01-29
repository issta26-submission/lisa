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
//<ID> 746
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
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON *optional_null = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(child, "name", name_item);
    cJSON_AddItemToObject(child, "optional", optional_null);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddNumberToObject(root, "id", 42.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(parsed, "child");
    cJSON *found_name = cJSON_GetObjectItem(detached, "name");
    const char *name_str = cJSON_GetStringValue(found_name);
    cJSON *found_optional = cJSON_GetObjectItem(detached, "optional");
    cJSON_bool optional_is_null = cJSON_IsNull(found_optional);
    (void)name_str;
    (void)optional_is_null;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}