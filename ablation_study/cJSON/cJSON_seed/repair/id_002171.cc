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
//<ID> 2171
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
    cJSON *number = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "number", number);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    cJSON *entry1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, entry1);
    cJSON *entry2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, entry2);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);

    // step 3: Operate and Validate
    int copy_size = cJSON_GetArraySize(items_copy);
    cJSON *size_item = cJSON_CreateNumber((double)copy_size);
    cJSON_AddItemToObject(root, "items_copy_size", size_item);
    cJSON *first = cJSON_GetArrayItem(items_copy, 0);
    cJSON *first_dup = cJSON_Duplicate(first, 0);
    cJSON_AddItemToObject(root, "first_dup", first_dup);
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
    cJSON_DeleteItemFromObject(root, "number");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}