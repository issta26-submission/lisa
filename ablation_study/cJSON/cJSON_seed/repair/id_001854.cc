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
//<ID> 1854
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
    cJSON_AddStringToObject(root, "label", "sample");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateString("a"));
    cJSON_AddItemToArray(items, cJSON_CreateString("b"));
    cJSON_AddItemToArray(items, cJSON_CreateString("c"));

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *meta_dup = cJSON_GetObjectItem(dup_root, "meta");
    cJSON *ver_dup = cJSON_GetObjectItem(meta_dup, "version");
    char *set_ret = cJSON_SetValuestring(ver_dup, "1.0-dup");

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int size_before = cJSON_GetArraySize(items);
    cJSON *item_b = cJSON_GetArrayItem(items, 1);
    cJSON *detached = cJSON_DetachItemViaPointer(items, item_b);
    int size_after_detach = cJSON_GetArraySize(items);
    cJSON_AddItemToArray(items, cJSON_CreateString("d"));
    cJSON_ReplaceItemInArray(items, 0, cJSON_CreateString("z"));
    cJSON_DeleteItemFromObject(root, "label");
    cJSON *ver_item = cJSON_GetObjectItem(meta, "version");
    const char *ver_str = cJSON_GetStringValue(ver_item);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver_str ? ver_str[0] : 'V';
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = (char)('0' + (size_after_detach % 10));
    buffer[4] = '\0';
    (void)set_ret; (void)buffer; (void)size_before; (void)size_after_detach; (void)ver_dup;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}