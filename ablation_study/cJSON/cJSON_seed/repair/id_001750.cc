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
//<ID> 1750
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "items", items);
    cJSON *entry = cJSON_CreateObject();
    cJSON_AddItemToArray(items, entry);
    cJSON *entry_name = cJSON_CreateString("entry1");
    cJSON_AddItemToObject(entry, "name", entry_name);
    cJSON *entry_value = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(entry, "value", entry_value);

    // step 2: Configure
    cJSON *entry_copy = cJSON_CreateObject();
    cJSON_AddItemToObject(entry_copy, "name", cJSON_CreateString("entry1"));
    cJSON_AddItemToObject(entry_copy, "value", cJSON_CreateNumber(42.0));
    cJSON_AddItemToObject(root, "metadata", cJSON_CreateObject());
    cJSON_AddItemToObject(root, "items_clone", cJSON_Duplicate(items, 1));

    // step 3: Operate and Validate
    cJSON_bool equal = cJSON_Compare(entry, entry_copy, 1);
    char *printed = cJSON_Print(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = equal ? '1' : '0';
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(entry_copy);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}