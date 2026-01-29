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
//<ID> 1799
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
    cJSON *first = cJSON_CreateObject();
    cJSON_AddItemToObject(first, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(first, "value", cJSON_CreateNumber(3.14));
    cJSON_AddItemToObject(first, "active", cJSON_CreateTrue());
    cJSON_AddItemToArray(items, first);

    // step 2: Configure
    cJSON *second = cJSON_Duplicate(first, 1);
    cJSON_AddItemToArray(items, second);
    cJSON_AddStringToObject(root, "config", "legacy");
    cJSON *newconf = cJSON_CreateObject();
    cJSON_AddItemToObject(newconf, "threshold", cJSON_CreateNumber(100.0));
    cJSON_AddItemToObject(newconf, "enabled", cJSON_CreateTrue());
    cJSON_ReplaceItemInObject(root, "config", newconf);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int count = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *name = cJSON_GetStringValue(cJSON_GetObjectItem(first_item, "name"));
    double value = cJSON_GetNumberValue(cJSON_GetObjectItem(first_item, "value"));
    double threshold = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "config"), "threshold"));
    cJSON_bool active_flag = cJSON_IsTrue(cJSON_GetObjectItem(first_item, "active"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name ? name[0] : 'Z';
    buffer[2] = (char)('0' + (count % 10));
    buffer[3] = (char)('0' + (((int)value) % 10));
    buffer[4] = (char)('0' + (((int)threshold) % 10));
    buffer[5] = active_flag ? '1' : '0';
    (void)buffer; (void)name; (void)value; (void)threshold; (void)active_flag;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}