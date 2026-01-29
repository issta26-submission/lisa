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
//<ID> 2177
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
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *count = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 2: Configure
    cJSON *entry1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, entry1);
    cJSON *entry2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, entry2);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_str ? first_str : "");
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON_bool copy_is_array = cJSON_IsArray(items_copy);
    cJSON *copy_flag = cJSON_CreateBool(copy_is_array);
    cJSON_AddItemToObject(root, "copy_is_array", copy_flag);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "count");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}