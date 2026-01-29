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
//<ID> 1866
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
    cJSON *ready = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "ready", ready);
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToArray(data, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(data, cJSON_CreateNumber(2.0));
    cJSON *item = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "item", item);
    cJSON_AddItemToObject(item, "name", cJSON_CreateString("initial"));

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(item, "name");
    char *set_ret = cJSON_SetValuestring(name_item, "configured");
    (void)set_ret;
    cJSON_ReplaceItemInArray(data, 1, cJSON_CreateString("two"));

    // step 3: Operate and Validate
    cJSON_bool is_ready = cJSON_IsTrue(cJSON_GetObjectItem(meta, "ready"));
    (void)is_ready;
    int size_before = cJSON_GetArraySize(data);
    cJSON_AddItemToArray(data, cJSON_CreateNumber(3.0));
    int size_after = cJSON_GetArraySize(data);
    cJSON *first = cJSON_GetArrayItem(data, 0);
    double val0 = cJSON_GetNumberValue(first);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (size_before % 10));
    buffer[2] = (char)('0' + (size_after % 10));
    buffer[3] = (char)('0' + ((int)val0 % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}