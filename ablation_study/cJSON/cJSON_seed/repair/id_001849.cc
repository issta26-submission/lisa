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
//<ID> 1849
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("1.2.3"));
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("sample"));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int size_before = cJSON_GetArraySize(items);
    cJSON *detached = cJSON_DetachItemFromArray(items, 1);
    int size_after = cJSON_GetArraySize(items);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *ver_item = cJSON_GetObjectItem(got_meta, "version");
    const char *ver_str = cJSON_GetStringValue(ver_item);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver_str ? ver_str[0] : 'V';
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = (char)('0' + (size_after % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}