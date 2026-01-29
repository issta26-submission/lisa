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
//<ID> 1855
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
    cJSON *version_item = cJSON_CreateString("1.0");
    cJSON_AddItemToObject(meta, "version", version_item);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON *item1 = cJSON_CreateString("beta");
    cJSON *item2 = cJSON_CreateString("gamma");
    cJSON_bool add0 = cJSON_AddItemToArray(items, item0);
    cJSON_bool add1 = cJSON_AddItemToArray(items, item1);
    cJSON_bool add2 = cJSON_AddItemToArray(items, item2);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("delta");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 1, replacement);
    char *new_ver = cJSON_SetValuestring(version_item, "1.1");

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(items);
    cJSON *to_detach = cJSON_GetArrayItem(items, 2);
    cJSON *detached = cJSON_DetachItemViaPointer(items, to_detach);
    int size_after = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromObject(root, "meta");
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = new_ver ? new_ver[0] : 'V';
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = (char)('0' + (size_after % 10));
    (void)add0; (void)add1; (void)add2; (void)replaced; (void)new_ver; (void)detached; (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}