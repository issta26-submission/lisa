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
//<ID> 1763
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(metrics, "values", values);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, num1);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, num2);

    // step 2: Configure
    double set1 = cJSON_SetNumberHelper(num1, 15.5);
    cJSON *inserted = cJSON_CreateNumber(12.0);
    cJSON_InsertItemInArray(values, 1, inserted);
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(cJSON_Version()));
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *replacement = cJSON_CreateNumber(99.9);
    cJSON_ReplaceItemViaPointer(values, first, replacement);
    double set_replacement = cJSON_SetNumberHelper(replacement, 42.42);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    char first_char = printed ? printed[0] : 'X';
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    buffer[0] = first_char;
    buffer[1] = (char)('0' + (int)set1 % 10);
    buffer[2] = (char)('0' + (int)set_replacement % 10);
    buffer[3] = (char)('0' + (int)v0 % 10);
    buffer[4] = (char)('0' + (int)v1 % 10);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}