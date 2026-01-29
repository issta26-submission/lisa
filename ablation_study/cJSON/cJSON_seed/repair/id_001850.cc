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
//<ID> 1850
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
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_meta = cJSON_GetObjectItem(dup_root, "meta");
    cJSON *dup_ver = cJSON_GetObjectItem(dup_meta, "version");
    cJSON_SetValuestring(dup_ver, "1.0-dup");
    cJSON *extra = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(items, extra);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(items, 1, replacement);

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *detached = cJSON_DetachItemViaPointer(items, first_item);
    cJSON_DeleteItemFromObject(meta, "version");
    char *printed = cJSON_PrintUnformatted(root);
    int printed_len = printed ? (int)strlen(printed) + 1 : 16;
    char *buffer = (char *)cJSON_malloc(printed_len);
    memset(buffer, 0, printed_len);
    strcpy(buffer, printed);
    cJSON_Minify(buffer);
    const char *dup_ver_str = cJSON_GetStringValue(dup_ver);
    double first_num_val = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));
    int size_items = cJSON_GetArraySize(items);
    buffer[0] = printed[0];
    buffer[1] = dup_ver_str[0];
    buffer[2] = (char)('0' + (size_items % 10));
    (void)first_num_val; (void)size_items; (void)dup_root; (void)replacement;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}