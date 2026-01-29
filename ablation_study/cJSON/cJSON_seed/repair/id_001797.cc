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
//<ID> 1797
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
    cJSON *entry1 = cJSON_CreateObject();
    cJSON_AddItemToObject(entry1, "enabled", cJSON_CreateTrue());
    cJSON_AddItemToArray(items, entry1);

    // step 2: Configure
    cJSON *entry_dup = cJSON_Duplicate(entry1, 1);
    cJSON_AddItemToArray(items, entry_dup);
    cJSON *replacement_array = cJSON_CreateArray();
    cJSON_AddItemToArray(replacement_array, cJSON_CreateTrue());
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "items", replacement_array);

    // step 3: Operate and Validate
    int array_size = cJSON_GetArraySize(replacement_array);
    cJSON *first_item = cJSON_GetArrayItem(replacement_array, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)first_is_true);
    buffer[2] = (char)('0' + (array_size % 10));
    (void)replaced;
    (void)entry_dup;
    (void)entry1;
    (void)items;
    (void)first_item;
    (void)array_size;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}