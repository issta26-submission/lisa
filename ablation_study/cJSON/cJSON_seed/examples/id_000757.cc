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
//<ID> 757
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
    cJSON *age_item = cJSON_CreateNumber(42.0);
    cJSON *nullable = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(child, "name", name_item);
    cJSON_AddItemToObject(child, "age", age_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "nullable", nullable);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON_bool has_child_flag = cJSON_HasObjectItem(parsed, "child");
    cJSON *detached_child = cJSON_DetachItemFromObjectCaseSensitive(parsed, "child");
    cJSON *detached_name = cJSON_GetObjectItem(detached_child, "name");
    const char *extracted_name = cJSON_GetStringValue(detached_name);
    (void)extracted_name;
    cJSON *parsed_nullable = cJSON_GetObjectItem(parsed, "nullable");
    cJSON_bool is_nullable_null = cJSON_IsNull(parsed_nullable);
    (void)has_child_flag;
    (void)is_nullable_null;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached_child);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}