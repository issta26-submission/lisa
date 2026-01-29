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
//<ID> 1857
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
    cJSON *ver_item = cJSON_CreateString("1.0");
    cJSON_AddItemToObject(meta, "version", ver_item);
    cJSON_AddStringToObject(root, "label", "sample");

    // step 2: Configure
    cJSON_SetValuestring(ver_item, "1.0-updated");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(40.0));

    // step 3: Operate
    cJSON *second_item_ptr = cJSON_GetArrayItem(items, 1);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 0, cJSON_CreateNumber(99.0));
    cJSON *detached_item = cJSON_DetachItemViaPointer(items, second_item_ptr);
    cJSON_DeleteItemFromObject(root, "label");

    // step 4: Validate and Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    int size_before = cJSON_GetArraySize(items);
    char *buffer = (char *)cJSON_malloc((size_t)strlen(printed) + 16);
    memset(buffer, 0, (size_t)strlen(printed) + 16);
    strcpy(buffer, printed);
    cJSON_Minify(buffer);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_ver = cJSON_GetObjectItem(got_meta, "version");
    const char *ver_str = cJSON_GetStringValue(got_ver);
    int size_after = cJSON_GetArraySize(items);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver_str ? ver_str[0] : 'V';
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = (char)('0' + (size_after % 10));
    buffer[4] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}