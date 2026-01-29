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
//<ID> 1696
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *target = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    double value_before = 0.0;
    double value_after = 0.0;
    int validation = 0;

    // step 2: Initialize
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    num2 = cJSON_CreateNumber(3.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    target = cJSON_GetArrayItem(arr, 1);
    value_before = cJSON_GetNumberValue(target);
    replacement = cJSON_CreateNumber(20.0);
    cJSON_ReplaceItemViaPointer(arr, target, replacement);
    size_after = cJSON_GetArraySize(arr);
    value_after = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));

    // step 5: Validate
    validation = (int)(size_before == 3) + (int)(size_after == 3) + (int)(value_before == 2.0) + (int)(value_after == 20.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(arr);
    (void)arr;
    (void)num0;
    (void)num1;
    (void)num2;
    (void)replacement;
    (void)target;
    (void)size_before;
    (void)size_after;
    (void)value_before;
    (void)value_after;
    // API sequence test completed successfully
    return 66;
}