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
//<ID> 2157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "items", items);
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello_world");

    // step 2: Configure
    cJSON *arr_entry = cJSON_CreateString("array_entry");
    cJSON_AddItemToArray(items, arr_entry);
    cJSON *duplicate = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObjectCS(root, "items_copy", duplicate);

    // step 3: Operate and Validate
    cJSON_bool dup_is_array = cJSON_IsArray(duplicate);
    cJSON *flag = cJSON_CreateBool(dup_is_array);
    cJSON_AddItemToObjectCS(root, "dup_is_array", flag);
    char *greeting_value = cJSON_GetStringValue(greeting);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = json[0];
    buffer[1] = greeting_value[0];
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "items_copy");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}