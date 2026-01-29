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
//<ID> 1858
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("1.0"));
    const char *ver_cstr = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "version"));
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *num10 = cJSON_CreateNumber(10.0);
    cJSON *num20 = cJSON_CreateNumber(20.0);
    cJSON *str_item = cJSON_CreateString("orig");
    cJSON_AddItemToArray(items, num10);
    cJSON_AddItemToArray(items, num20);
    cJSON_AddItemToArray(items, str_item);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(items);
    cJSON_ReplaceItemInArray(items, 0, cJSON_CreateNumber(100.0));
    cJSON *detached = cJSON_DetachItemViaPointer(items, num20);
    cJSON_SetValuestring(str_item, "updated");

    // step 3: Operate and Validate
    int size_after = cJSON_GetArraySize(items);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver_cstr ? ver_cstr[0] : 'V';
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = (char)('0' + (size_after % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_DeleteItemFromObject(root, "meta");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}