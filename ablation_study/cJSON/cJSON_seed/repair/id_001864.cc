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
//<ID> 1864
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
    cJSON *name = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "enabled", flag);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.0));

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(meta, "name");
    char *set_ret = cJSON_SetValuestring(name_item, "beta");
    cJSON_bool flag_true = cJSON_IsTrue(flag);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int size_before = cJSON_GetArraySize(arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("extra"));
    int size_after = cJSON_GetArraySize(arr);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)flag_true);
    buffer[2] = set_ret ? set_ret[0] : 'S';
    buffer[3] = (char)('0' + (size_before % 10));
    buffer[4] = (char)('0' + (size_after % 10));
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}