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
//<ID> 1755
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
    cJSON *entry2 = cJSON_CreateObject();
    cJSON_AddItemToArray(items, entry1);
    cJSON_AddItemToArray(items, entry2);
    cJSON_AddItemToObject(entry1, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(entry1, "value", cJSON_CreateNumber(10.0));
    cJSON_AddItemToObject(entry2, "name", cJSON_CreateString("Alpha"));
    cJSON_AddItemToObject(entry2, "value", cJSON_CreateNumber(20.0));

    // step 2: Configure
    cJSON_AddItemToObject(root, "config", cJSON_CreateObject());
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON_AddItemToObject(config, "enabled", cJSON_CreateTrue());
    cJSON_AddStringToObject(root, "version", cJSON_Version());

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal_cs = cJSON_Compare(root, root_copy, 1);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *first_name_node = cJSON_GetObjectItem(first_item, "name");
    const char *first_name = cJSON_GetStringValue(first_name_node);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = first_name ? first_name[0] : 'Y';
    buffer[2] = equal_cs ? '1' : '0';
    (void)buffer;
    (void)first_name;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}