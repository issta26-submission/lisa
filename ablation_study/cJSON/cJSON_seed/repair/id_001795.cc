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
//<ID> 1795
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *objA = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "objA", objA);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag_true);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(objA, "items", items);

    // step 2: Configure
    cJSON *item1 = cJSON_CreateObject();
    cJSON_AddStringToObject(item1, "name", "first");
    cJSON_AddNumberToObject(item1, "id", 101.0);
    cJSON_AddItemToArray(items, item1);
    cJSON *dup_item = cJSON_Duplicate(item1, 1);
    cJSON_AddItemToArray(items, dup_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_ReplaceItemInObject(meta, "version", cJSON_CreateNumber(2.0));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_name = cJSON_GetStringValue(cJSON_GetObjectItem(first_item, "name"));
    int count = cJSON_GetArraySize(items);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = first_name ? first_name[0] : 'Z';
    buffer[2] = (char)('0' + (count % 10));
    (void)first_name;
    (void)count;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}