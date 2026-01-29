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
//<ID> 1843
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
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(84.0);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);
    cJSON *label = cJSON_CreateString("test-label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    const char *lib_ver = cJSON_Version();
    cJSON *meta_info = cJSON_CreateString("metadata");
    cJSON_AddItemToObject(meta, "info", meta_info);

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    int size_before = cJSON_GetArraySize(got_items);
    cJSON *detached = cJSON_DetachItemFromArray(got_items, 0);
    cJSON_AddItemToObject(root, "moved", detached);
    int size_after = cJSON_GetArraySize(got_items);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *label_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = lib_ver ? lib_ver[0] : 'V';
    buffer[2] = label_str ? label_str[0] : 'L';
    buffer[3] = (char)('0' + (size_before % 10));
    buffer[4] = (char)('0' + (size_after % 10));
    (void)buffer; (void)label_str; (void)size_before; (void)size_after; (void)lib_ver;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}