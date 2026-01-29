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
//<ID> 1846
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
    cJSON *ver_str = cJSON_CreateString("1.2.3");
    cJSON_AddItemToObject(meta, "version", ver_str);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON *num3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);
    cJSON_AddItemToArray(items, num3);

    // step 2: Configure
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    int size_before = cJSON_GetArraySize(items_ref);
    cJSON *detached = cJSON_DetachItemFromArray(items_ref, 1);
    cJSON_AddItemToObject(root, "moved", detached);
    int size_after = cJSON_GetArraySize(items_ref);
    cJSON *moved_item = cJSON_GetObjectItem(root, "moved");
    double moved_val = cJSON_GetNumberValue(moved_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const char *libver = cJSON_Version();
    size_t buf_len = (printed ? strlen(printed) : 0) + 32;
    char *buffer = (char *)cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    if (printed) strcpy(buffer, printed);
    cJSON_Minify(buffer);
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *version_item = cJSON_GetObjectItem(meta_ref, "version");
    const char *version_str = cJSON_GetStringValue(version_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = libver ? libver[0] : 'V';
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = (char)('0' + (size_after % 10));
    buffer[4] = (char)('0' + (((int)moved_val) % 10));
    buffer[5] = version_str ? version_str[0] : 'v';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}