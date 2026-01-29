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
//<ID> 1897
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON *str3 = cJSON_CreateString("thirty");
    cJSON_InsertItemInArray(array, 0, num1);
    cJSON_InsertItemInArray(array, 1, num2);
    cJSON_InsertItemInArray(array, 1, str3);

    // step 2: Configure
    cJSON *middle = cJSON_GetArrayItem(array, 1);
    cJSON_bool middle_is_string = cJSON_IsString(middle);
    cJSON *meta = cJSON_CreateString("v1");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *meta_new = cJSON_CreateString("v2");
    cJSON_ReplaceItemInObject(root, "meta", meta_new);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (middle_is_string % 10));
    buffer[2] = (char)('0' + (cJSON_GetArraySize(array) % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}