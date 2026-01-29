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
//<ID> 318
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
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(7.5);
    cJSON *false_item = cJSON_CreateFalse();
    int ints[3] = {10, 20, 30};
    cJSON *int_arr = cJSON_CreateIntArray(ints, 3);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, int_arr);

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "enabled", false_item);
    cJSON_AddItemToObject(root, "pi", cJSON_CreateNumber(3.14159));

    // step 3: Operate & Validate
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON *inner_second = cJSON_GetArrayItem(int_arr, 1);
    double sum = cJSON_GetNumberValue(first_elem) + cJSON_GetNumberValue(inner_second);
    cJSON_AddItemToObject(root, "sum_first_and_inner_second", cJSON_CreateNumber(sum));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(cJSON_GetNumberValue(first_elem) * 2.0));
    cJSON *newly_appended = cJSON_GetArrayItem(arr, 3);
    double appended_val = cJSON_GetNumberValue(newly_appended);
    cJSON_AddItemToObject(root, "appended_times_ten", cJSON_CreateNumber(appended_val * 10.0));

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}