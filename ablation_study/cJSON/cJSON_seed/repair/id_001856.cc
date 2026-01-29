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
//<ID> 1856
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateString("extra"));
    cJSON *ver_item = cJSON_GetObjectItem(meta, "version");
    char *new_vs = cJSON_SetValuestring(ver_item, "1.0.1");
    (void)new_vs;
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(items, 1, replacement);

    // step 3: Operate and Validate
    cJSON *third = cJSON_GetArrayItem(items, 2);
    cJSON *detached = cJSON_DetachItemViaPointer(items, third);
    cJSON_DeleteItemFromObject(root, "meta");
    char *printed = cJSON_Print(root);
    int size_before = cJSON_GetArraySize(items);
    char *buffer = (char *)cJSON_malloc((int)strlen(printed) + 16);
    memset(buffer, 0, (int)strlen(printed) + 16);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver_item && ver_item->valuestring ? ver_item->valuestring[0] : 'V';
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}