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
//<ID> 1762
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
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(arr, 0, num1);
    cJSON *num2 = cJSON_CreateNumber(7.0);
    cJSON_InsertItemInArray(arr, 1, num2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *replacement = cJSON_CreateNumber(0.0);
    double setv = cJSON_SetNumberHelper(replacement, 99.5);
    (void)setv;

    // step 3: Operate and Validate
    cJSON_ReplaceItemViaPointer(arr, num1, replacement);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double val = cJSON_GetNumberValue(first);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    int digit = (int)val % 10;
    buffer[1] = (char)('0' + (digit < 0 ? -digit : digit));
    (void)digit;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}