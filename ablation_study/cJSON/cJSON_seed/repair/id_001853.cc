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
//<ID> 1853
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
    cJSON *ver = cJSON_CreateString("1.0");
    cJSON_AddItemToObject(meta, "version", ver);

    // step 2: Configure
    char *sv = cJSON_SetValuestring(ver, "1.2.0");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(99.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(100.0));
    int size_before = cJSON_GetArraySize(items);

    // step 3: Operate and Validate
    cJSON *item0 = cJSON_GetArrayItem(items, 0);
    cJSON *detached = cJSON_DetachItemViaPointer(items, item0);
    cJSON_AddItemToArray(items, detached);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 1, cJSON_CreateNumber(42.0));
    char *printed = cJSON_PrintUnformatted(root);
    int size_after = cJSON_GetArraySize(items);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = sv ? sv[0] : 'V';
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = (char)('0' + (size_after % 10));
    buffer[4] = '\0';
    (void)replaced;

    cJSON_DeleteItemFromObject(meta, "version");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}