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
//<ID> 2158
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

    // step 2: Configure
    cJSON *answer = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObjectCS(root, "answer", answer);
    cJSON *entry1 = cJSON_CreateString("entry_one");
    cJSON *entry2 = cJSON_CreateString("entry_two");
    cJSON_AddItemToArray(items, entry1);
    cJSON_AddItemToArray(items, entry2);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObjectCS(root, "items_copy", items_copy);

    // step 3: Operate and Validate
    cJSON *got_greeting = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    char *greeting_val = cJSON_GetStringValue(got_greeting);
    cJSON *copied = cJSON_GetObjectItemCaseSensitive(root, "items_copy");
    cJSON_bool copied_is_array = cJSON_IsArray(copied);
    cJSON *flag = cJSON_CreateBool(copied_is_array);
    cJSON_AddItemToObjectCS(root, "copied_is_array", flag);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = json[0];
    buffer[1] = json[1];
    buffer[2] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "greeting");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}