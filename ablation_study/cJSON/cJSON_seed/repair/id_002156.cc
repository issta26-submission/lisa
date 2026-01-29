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
//<ID> 2156
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
    cJSON_AddItemToObject(root, "items", items);
    cJSON *greeting = cJSON_CreateString("hello_world");
    cJSON_AddItemToObjectCS(root, "greeting", greeting);
    cJSON *temp = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObjectCS(root, "temp", temp);

    // step 2: Configure
    cJSON *entry = cJSON_CreateString("array_entry");
    cJSON_AddItemToArray(items, entry);
    cJSON *duplicate = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObjectCS(root, "items_copy", duplicate);

    // step 3: Operate and Validate
    cJSON_bool dup_is_array = cJSON_IsArray(duplicate);
    cJSON *flag = cJSON_CreateBool(dup_is_array);
    cJSON_AddItemToObject(root, "dup_is_array", flag);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    char *greet_val = cJSON_GetStringValue(greeting);
    buffer[0] = greet_val[0];
    buffer[1] = '\0';
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}