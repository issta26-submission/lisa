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
//<ID> 2179
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
    cJSON *title = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "title", title);

    // step 2: Configure
    cJSON *entry1 = cJSON_CreateString("first");
    cJSON_AddItemToArray(items, entry1);
    cJSON *entry2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(items, entry2);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON_bool copy_is_array = cJSON_IsArray(items_copy);
    cJSON *marker = cJSON_CreateBool(copy_is_array);
    cJSON_AddItemToObject(meta, "items_copy_is_array", marker);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
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