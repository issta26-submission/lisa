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
//<ID> 357
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *bool_true = cJSON_CreateTrue();
    cJSON *bool_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON_AddItemToArray(values, bool_true);
    cJSON_AddItemToArray(values, bool_false);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "pi_literal", 3.14159);

    // step 2: Configure
    int count = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "values_count", (double)count);
    cJSON_bool is_true_bool = cJSON_IsBool(bool_true);
    cJSON_bool is_false_bool = cJSON_IsBool(bool_false);
    cJSON_bool true_is_true = cJSON_IsTrue(bool_true);
    cJSON_AddNumberToObject(root, "true_is_bool", (double)is_true_bool);
    cJSON_AddNumberToObject(root, "false_is_bool", (double)is_false_bool);
    cJSON_AddNumberToObject(root, "true_is_true", (double)true_is_true);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 512, 0);
    cJSON_AddNumberToObject(root, "prealloc_ok", (double)prealloc_ok);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}