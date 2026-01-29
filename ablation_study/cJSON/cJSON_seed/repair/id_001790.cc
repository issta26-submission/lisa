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
//<ID> 1790
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
    cJSON *t_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, t_true);
    cJSON *num_item = cJSON_CreateNumber(13.5);
    cJSON_AddItemToArray(items, num_item);
    cJSON *str_item = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(items, str_item);
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(settings, "enabled", cJSON_CreateTrue());
    cJSON_AddItemToObject(settings, "level", cJSON_CreateNumber(2.0));

    // step 2: Configure
    cJSON *settings_dup = cJSON_Duplicate(settings, 1);
    cJSON_ReplaceItemInObject(root, "settings", settings_dup);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "active", cJSON_CreateTrue());

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int count = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *third = cJSON_GetArrayItem(items, 2);
    const char *third_str = cJSON_GetStringValue(third);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (count % 10));
    buffer[2] = third_str ? third_str[0] : 'Z';
    buffer[3] = cJSON_IsTrue(first) ? 'T' : 'F';
    (void)buffer;
    (void)count;
    (void)third_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}