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
//<ID> 1482
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *base = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "base", base);

    // step 2: Configure
    char *printed_root = cJSON_PrintUnformatted(root);
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    cJSON_AddItemToObject(root, "is_array", cJSON_CreateBool(arr_is_array));

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    double base_val = cJSON_GetNumberValue(base);
    double *stored = (double *)cJSON_malloc(sizeof(double));
    *stored = first_val + base_val;
    cJSON_AddItemToObject(root, "sum", cJSON_CreateNumber(*stored));
    char *printed_sum_item = cJSON_PrintUnformatted(cJSON_GetObjectItem(root, "sum"));

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_sum_item);
    cJSON_free(stored);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)first_val;
    (void)arr_is_array;
    return 66;
}