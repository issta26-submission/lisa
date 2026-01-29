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
//<ID> 743
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
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("example");
    (void)version;

    // step 2: Configure
    cJSON_AddItemToObject(child, "num", num_item);
    cJSON_AddItemToObject(child, "str", str_item);
    cJSON_AddItemToObject(child, "nil", null_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "title", cJSON_CreateString("root_title"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *found_child = cJSON_GetObjectItem(parsed, "child");
    (void)found_child;
    cJSON *detached_child = cJSON_DetachItemFromObjectCaseSensitive(parsed, "child");
    cJSON *detached_num = cJSON_GetObjectItem(detached_child, "num");
    double extracted_num = cJSON_GetNumberValue(detached_num);
    cJSON *detached_str_item = cJSON_GetObjectItem(detached_child, "str");
    const char *extracted_str = cJSON_GetStringValue(detached_str_item);
    (void)extracted_num;
    (void)extracted_str;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached_child);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}