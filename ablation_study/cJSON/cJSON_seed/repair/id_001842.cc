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
//<ID> 1842
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToArray(items, n3);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", "v1.0");

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_meta = cJSON_GetObjectItem(dup_root, "meta");
    cJSON *dup_ver_item = cJSON_GetObjectItem(dup_meta, "version");
    cJSON_SetValuestring(dup_ver_item, "v1.0-dup");

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    int size_before = cJSON_GetArraySize(got_items);
    cJSON *detached = cJSON_DetachItemFromArray(got_items, 1);
    cJSON_AddItemToObject(meta, "detached", detached);
    int size_after = cJSON_GetArraySize(got_items);
    char *printed = cJSON_PrintUnformatted(root);
    int printed_len = printed ? (int)strlen(printed) : 0;
    char *buffer = (char *)cJSON_malloc((size_t)printed_len + 16);
    memset(buffer, 0, (size_t)printed_len + 16);
    if (printed) strcpy(buffer, printed);
    cJSON_Minify(buffer);
    cJSON *version_item = cJSON_GetObjectItem(meta, "version");
    const char *version_str = cJSON_GetStringValue(version_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = version_str ? version_str[0] : 'V';
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = (char)('0' + (size_after % 10));
    (void)dup_ver_item; (void)dup_meta; (void)version_item; (void)got_items;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}