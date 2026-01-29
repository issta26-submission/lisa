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
//<ID> 1748
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
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *value = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(config, "value", value);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20));

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "note", "configured");
    cJSON *tag = cJSON_CreateString("v1");
    cJSON_AddItemToObject(root, "tag", tag);

    // step 3: Operate and Validate
    cJSON *config_ptr = cJSON_GetObjectItem(root, "config");
    cJSON *detached_config = cJSON_DetachItemFromObject(root, "config");
    cJSON_bool config_invalid = cJSON_IsInvalid(detached_config);
    cJSON *items_ptr = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items_ptr, 0);
    cJSON *detached_first = cJSON_DetachItemViaPointer(items_ptr, first_item);
    cJSON_bool first_invalid = cJSON_IsInvalid(detached_first);
    char *printed = cJSON_PrintUnformatted(root);
    const char *version = cJSON_Version();
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = version ? version[0] : 'V';
    buffer[2] = (char)('0' + (int)config_invalid);
    buffer[3] = (char)('0' + (int)first_invalid);
    (void)config_ptr;
    (void)detached_config;
    (void)detached_first;
    (void)items_ptr;
    (void)first_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached_config);
    cJSON_Delete(detached_first);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}