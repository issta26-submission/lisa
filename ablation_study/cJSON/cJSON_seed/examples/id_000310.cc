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
//<ID> 310
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
    const int numbers[] = {10, 20, 30};
    cJSON *int_arr = cJSON_CreateIntArray(numbers, 3);
    cJSON *num_item = cJSON_CreateNumber(7.5);
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, int_arr);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate & Validate
    cJSON *nested = cJSON_GetArrayItem(arr, 0);
    cJSON *nested_num = cJSON_GetArrayItem(nested, 1);
    double extracted = cJSON_GetNumberValue(nested_num);
    cJSON *computed = cJSON_CreateNumber(extracted * 2.0);
    cJSON_AddItemToArray(arr, computed);
    cJSON *check_item = cJSON_GetArrayItem(arr, 3);
    double check_value = cJSON_GetNumberValue(check_item);
    cJSON *final_num = cJSON_CreateNumber(check_value + 1.0);
    cJSON_AddItemToArray(arr, final_num);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}