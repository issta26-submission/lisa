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
//<ID> 355
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(values, flag_true);

    // step 2: Configure
    int count = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "count", (double)count);
    double sum = cJSON_GetNumberValue(num1) + cJSON_GetNumberValue(num2);
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 3: Operate
    cJSON_bool is_bool = cJSON_IsBool(flag_true);
    cJSON_bool is_true = cJSON_IsTrue(flag_true);
    cJSON_AddNumberToObject(root, "is_bool", (double)is_bool);
    cJSON_AddNumberToObject(root, "is_true", (double)is_true);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_AddNumberToObject(root, "prealloc_ok", (double)prealloc_ok);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}