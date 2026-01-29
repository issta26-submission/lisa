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
//<ID> 1759
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
    cJSON *item1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, item1);
    cJSON *item2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, item2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "env", "production");
    cJSON_AddStringToObject(root, "cjson_version", cJSON_Version());

    // step 3: Operate and Validate
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_bool identical = cJSON_Compare(config, config_copy, 1);
    char *printed = cJSON_Print(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *first_item_str = cJSON_GetStringValue(item1);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)identical);
    buffer[2] = first_item_str ? first_item_str[0] : 'Z';
    (void)buffer;
    (void)first_item_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(config_copy);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}