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
//<ID> 1764
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "numbers", numbers);
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON *num1 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(numbers, num0);
    cJSON_AddItemToArray(numbers, num1);

    // step 2: Configure
    cJSON *inserted = cJSON_CreateNumber(7.0);
    cJSON_InsertItemInArray(numbers, 1, inserted);
    double set_ret = cJSON_SetNumberHelper(num0, 100.5);
    (void)set_ret;
    cJSON *replacement = cJSON_CreateNumber(9.9);
    cJSON_ReplaceItemViaPointer(numbers, num1, replacement);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *a0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *a1 = cJSON_GetArrayItem(numbers, 1);
    cJSON *a2 = cJSON_GetArrayItem(numbers, 2);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double v2 = cJSON_GetNumberValue(a2);
    double total = v0 + v1 + v2;
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + ((int)total % 10));
    buffer[2] = (char)('0' + ((int)v0 % 10));
    buffer[3] = (char)('0' + ((int)v1 % 10));
    buffer[4] = (char)('0' + ((int)v2 % 10));
    (void)buffer;
    (void)total;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}