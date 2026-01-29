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
//<ID> 1768
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str1);

    // step 2: Configure
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(arr, 1, num2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *countItem = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(meta, "count", countItem);
    double new_count = cJSON_SetNumberHelper(countItem, 3.0);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateString("world");
    cJSON_ReplaceItemViaPointer(arr, str1, replacement);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + ((int)new_count % 10));
    buffer[2] = (char)('0' + ((int)cJSON_GetNumberValue(num1) % 10));
    (void)buffer;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}