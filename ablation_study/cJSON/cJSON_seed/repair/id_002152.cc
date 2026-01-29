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
//<ID> 2152
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
    cJSON *meta = cJSON_AddStringToObject(root, "meta", "hello_world");
    cJSON *num = cJSON_CreateNumber(2026.0);
    cJSON_AddItemToObject(root, "year", num);

    // step 2: Configure
    cJSON *arr_entry = cJSON_CreateString("array_entry");
    cJSON_AddItemToArray(items, arr_entry);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);
    const char *meta_str = cJSON_GetStringValue(meta);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = meta_str[0];
    buffer[1] = meta_str[1];
    buffer[2] = '\0';

    // step 3: Operate and Validate
    cJSON_bool dup_is_array = cJSON_IsArray(items_copy);
    cJSON *flag = cJSON_CreateBool(dup_is_array);
    cJSON_AddItemToObject(root, "dup_is_array", flag);
    char *json = cJSON_PrintUnformatted(root);
    buffer[0] = json[0];
    buffer[1] = json[1];
    buffer[2] = json[2];
    buffer[3] = '\0';
    cJSON_DeleteItemFromObjectCaseSensitive(root, "meta");

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}