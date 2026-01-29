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
//<ID> 1894
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
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(arr, 0, num0);
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON_InsertItemInArray(arr, 1, str1);
    cJSON *bool2 = cJSON_CreateTrue();
    cJSON_InsertItemInArray(arr, 2, bool2);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "name", cJSON_CreateString("original"));

    // step 2: Configure
    cJSON *got0 = cJSON_GetArrayItem(arr, 0);
    cJSON *got1 = cJSON_GetArrayItem(arr, 1);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(child, "name", replacement);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    double value0 = cJSON_GetNumberValue(got0);
    char *s1 = cJSON_GetStringValue(got1);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + ((int)value0 % 10));
    buffer[2] = s1 ? s1[0] : 'X';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}