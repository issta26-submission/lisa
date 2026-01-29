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
//<ID> 539
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
    cJSON_AddStringToObject(root, "title", "example");
    cJSON_AddNullToObject(root, "optional");
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(list, s1);
    cJSON_AddItemToArray(list, s2);

    // step 3: Operate & Validate
    cJSON_bool was_array = cJSON_IsArray(list);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_list = cJSON_GetObjectItem(parsed, "list");
    cJSON_bool parsed_is_array = cJSON_IsArray(parsed_list);
    cJSON_DeleteItemFromArray(list, 0);
    cJSON *s3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(list, s3);
    int size_after = cJSON_GetArraySize(list);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;was_array=%d;parsed_is_array=%d;size=%d",
            version ? version : "null",
            (int)was_array,
            (int)parsed_is_array,
            size_after);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}