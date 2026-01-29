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
//<ID> 1839
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    const char *lib_ver = cJSON_Version();
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *meta_dup = cJSON_GetObjectItem(dup_root, "meta");
    cJSON *ver_item_dup = cJSON_GetObjectItem(meta_dup, "version");
    cJSON_SetValuestring(ver_item_dup, "1.0-dup");

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    int size_before = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromArray(items, 1);
    int size_after = cJSON_GetArraySize(items);
    char *buffer = (char *)cJSON_malloc((int)strlen(printed) + 1);
    memset(buffer, 0, (int)strlen(printed) + 1);
    strcpy(buffer, printed);
    cJSON_Minify(buffer);
    cJSON *version_item = cJSON_GetObjectItem(meta, "version");
    const char *version_str = cJSON_GetStringValue(version_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = lib_ver ? lib_ver[0] : 'V';
    buffer[2] = version_str ? version_str[0] : 'v';
    buffer[3] = (char)('0' + (size_before % 10));
    buffer[4] = (char)('0' + (size_after % 10));
    (void)buffer; (void)size_before; (void)size_after; (void)ver_item_dup;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}