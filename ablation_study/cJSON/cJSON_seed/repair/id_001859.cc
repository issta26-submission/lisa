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
//<ID> 1859
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
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("0.1"));

    // step 2: Configure
    cJSON *ver_item = cJSON_GetObjectItem(meta, "version");
    char *set_ret = cJSON_SetValuestring(ver_item, "0.2");
    (void)set_ret;
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *detached = cJSON_DetachItemViaPointer(arr, first);
    cJSON_DeleteItemFromObject(meta, "version");
    char *printed = cJSON_PrintUnformatted(root);
    int size_before = cJSON_GetArraySize(arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("extra"));
    int size_after = cJSON_GetArraySize(arr);
    double val0 = 0.0;
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    val0 = cJSON_GetNumberValue(item0);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (size_before % 10));
    buffer[2] = (char)('0' + (size_after % 10));
    buffer[3] = (char)('0' + ((int)val0 % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}