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
//<ID> 1852
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
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("1.0"));

    // step 2: Configure
    cJSON *ver_item = cJSON_GetObjectItem(meta, "version");
    char *new_ver = cJSON_SetValuestring(ver_item, "2.0");

    // step 3: Operate and Validate
    cJSON *detached_ver = cJSON_DetachItemViaPointer(meta, ver_item);
    cJSON_AddItemToArray(items, detached_ver);
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(items, 1, replacement);
    cJSON_DeleteItemFromObject(root, "meta");
    char *printed = cJSON_Print(root);
    int arr_size = cJSON_GetArraySize(items);
    cJSON *str_item = cJSON_GetArrayItem(items, 3);
    const char *ver_str = cJSON_GetStringValue(str_item);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver_str ? ver_str[0] : 'V';
    buffer[2] = (char)('0' + (arr_size % 10));
    (void)new_ver;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}