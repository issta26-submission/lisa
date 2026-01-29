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
//<ID> 289
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num);
    cJSON *bool_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, bool_item);
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddTrueToObject(inner, "flag");
    cJSON_AddNumberToObject(inner, "value", 7.0);

    // step 2: Configure
    cJSON *arr_ref = cJSON_CreateArrayReference(inner);
    cJSON_AddItemToArray(arr, arr_ref);
    cJSON *extra_num = cJSON_CreateNumber(99.99);
    cJSON_AddItemToObject(root, "extra", extra_num);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool second_is_bool = cJSON_IsBool(second);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    cJSON_AddNumberToObject(root, "second_is_bool", (double)second_is_bool);
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}