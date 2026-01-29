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
//<ID> 1104
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag);
    double sum = cJSON_GetNumberValue(n1) + cJSON_GetNumberValue(n2);
    cJSON *sum_item = cJSON_AddNumberToObject(root, "sum", sum);

    // step 3: Operate & Validate
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    int size = cJSON_GetArraySize(arr);
    cJSON *count_item = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON *is_array_item = cJSON_AddNumberToObject(root, "is_array", (double)arr_is_array);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)sum_item;
    (void)is_array_item;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}