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
//<ID> 2159
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
    cJSON *number = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObjectCS(root, "number", number);

    // step 2: Configure
    cJSON *entry1 = cJSON_CreateString("entry_one");
    cJSON_AddItemToArray(items, entry1);
    cJSON *entry2 = cJSON_CreateString("entry_two");
    cJSON_AddItemToArray(items, entry2);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObjectCS(root, "items_copy", items_copy);

    // step 3: Operate and Validate
    cJSON_bool copy_is_array = cJSON_IsArray(items_copy);
    cJSON *flag = cJSON_CreateBool(copy_is_array);
    cJSON_AddItemToObjectCS(root, "copy_is_array", flag);
    cJSON *greet_item = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    char *greet_val = cJSON_GetStringValue(greet_item);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (greet_val && greet_val[0]) ? greet_val[0] : '\0';
    buffer[1] = (json && json[0]) ? json[0] : '\0';
    buffer[2] = (json && json[1]) ? json[1] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "number");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}