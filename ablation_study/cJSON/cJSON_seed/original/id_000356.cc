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
//<ID> 356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);
    cJSON_AddItemToArray(items, true_item);
    cJSON_AddItemToArray(items, false_item);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double sum = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON *third = cJSON_GetArrayItem(items, 2);
    cJSON_bool third_is_bool = cJSON_IsBool(third);
    cJSON_bool third_is_true = cJSON_IsTrue(third);
    cJSON_AddNumberToObject(root, "third_is_bool", (double)third_is_bool);
    cJSON_AddNumberToObject(root, "third_is_true", (double)third_is_true);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON_AddNumberToObject(root, "prealloc_ok", (double)prealloc_ok);
    cJSON *fourth = cJSON_GetArrayItem(items, 3);
    cJSON_bool fourth_is_bool = cJSON_IsBool(fourth);
    cJSON_bool fourth_is_true = cJSON_IsTrue(fourth);
    cJSON_AddNumberToObject(root, "fourth_is_bool", (double)fourth_is_bool);
    cJSON_AddNumberToObject(root, "fourth_is_true", (double)fourth_is_true);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}