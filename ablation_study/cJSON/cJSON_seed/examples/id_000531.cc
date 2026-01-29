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
//<ID> 531
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
    cJSON_AddStringToObject(root, "title", "example");
    cJSON_AddNullToObject(root, "maybe");
    cJSON *s0 = cJSON_CreateString("one");
    cJSON *s1 = cJSON_CreateString("two");
    cJSON *s2 = cJSON_CreateString("three");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_bool was_array = cJSON_IsArray(items);

    // step 3: Operate & Validate
    char *snapshot1 = cJSON_PrintUnformatted(root);
    cJSON *parsed1 = cJSON_Parse(snapshot1);
    cJSON *parsed_items1 = cJSON_GetObjectItem(parsed1, "items");
    cJSON *first_parsed = cJSON_GetArrayItem(parsed_items1, 0);
    const char *first_str = cJSON_GetStringValue(first_parsed);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s;first=%s;was_array=%d",
            version ? version : "unknown",
            first_str ? first_str : "null",
            (int)was_array);

    // modify original array: remove the second element ("two")
    cJSON_DeleteItemFromArray(items, 1);
    char *snapshot2 = cJSON_PrintUnformatted(root);
    cJSON *parsed2 = cJSON_Parse(snapshot2);
    cJSON *parsed_items2 = cJSON_GetObjectItem(parsed2, "items");
    cJSON_bool is_array_after = cJSON_IsArray(parsed_items2);

    // step 4: Cleanup
    cJSON_free(snapshot1);
    cJSON_free(snapshot2);
    cJSON_Delete(parsed1);
    cJSON_Delete(parsed2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}