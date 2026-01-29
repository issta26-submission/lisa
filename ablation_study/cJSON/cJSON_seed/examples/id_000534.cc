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
//<ID> 534
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *s0 = cJSON_CreateString("one");
    cJSON *s1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddNullToObject(root, "maybe");
    cJSON *status_item = cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON_bool parsed_is_array = cJSON_IsArray(parsed_items);
    cJSON *first_parsed = cJSON_GetArrayItem(parsed_items, 0);
    const char *first_str = cJSON_GetStringValue(first_parsed);
    cJSON_DeleteItemFromArray(items, 0);
    int size_after = cJSON_GetArraySize(items);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s;first=%s;parsed_is_array=%d;size_after=%d",
            version ? version : "unknown",
            first_str ? first_str : "null",
            (int)parsed_is_array,
            size_after);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}