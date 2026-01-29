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
//<ID> 530
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
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON_AddStringToObject(root, "title", "example-list");
    cJSON_AddNullToObject(root, "optional");
    cJSON *item0 = cJSON_CreateString("one");
    cJSON *item1 = cJSON_CreateString("two");
    cJSON *item2 = cJSON_CreateString("three");
    cJSON_AddItemToArray(list, item0);
    cJSON_AddItemToArray(list, item1);
    cJSON_AddItemToArray(list, item2);

    // step 3: Operate & Validate
    cJSON_bool is_array_before = cJSON_IsArray(list);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_list = cJSON_GetObjectItem(parsed, "list");
    cJSON_bool parsed_is_array = cJSON_IsArray(parsed_list);
    cJSON_DeleteItemFromArray(parsed_list, 1);
    cJSON *first_item = cJSON_GetArrayItem(parsed_list, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s;is_array_before=%d;parsed_is_array=%d;first=%s",
            version ? version : "unknown",
            (int)is_array_before,
            (int)parsed_is_array,
            first_str ? first_str : "null");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}