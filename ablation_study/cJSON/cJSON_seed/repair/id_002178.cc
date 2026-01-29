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
//<ID> 2178
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
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer);

    // step 2: Configure
    cJSON *entry1 = cJSON_CreateString("first_entry");
    cJSON_AddItemToArray(items, entry1);
    cJSON *entry2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, entry2);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);
    cJSON_DeleteItemFromObject(root, "items");

    // step 3: Operate and Validate
    int copy_size = cJSON_GetArraySize(items_copy);
    cJSON *first_item = cJSON_GetArrayItem(items_copy, 0);
    char *first_val = cJSON_GetStringValue(first_item);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (first_val && first_val[0]) ? first_val[0] : '\0';
    buffer[1] = (json && json[0]) ? json[0] : '\0';
    buffer[2] = (json && json[1]) ? json[1] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);
    (void)copy_size;

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "answer");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}