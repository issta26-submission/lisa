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
//<ID> 2154
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
    cJSON *greeting = cJSON_CreateString("hello_world");
    cJSON_AddItemToObjectCS(root, "greeting", greeting);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObjectCS(root, "items_copy", items_copy);

    // step 2: Configure
    cJSON *entry = cJSON_CreateString("array_entry");
    cJSON_AddItemToArray(items, entry);
    cJSON_bool copy_is_array = cJSON_IsArray(items_copy);
    cJSON *flag = cJSON_CreateBool(copy_is_array);
    cJSON_AddItemToObjectCS(root, "copy_is_array", flag);

    // step 3: Operate and Validate
    char *greeting_str = cJSON_GetStringValue(greeting);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = greeting_str[0];
    buffer[1] = greeting_str[1];
    buffer[2] = '\0';
    cJSON_DeleteItemFromObjectCaseSensitive(root, "items_copy");

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}