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
//<ID> 1699
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
    double val0 = 0.0;
    double val1 = 0.0;
    double val2 = 0.0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    num2 = cJSON_CreateNumber(3.0);
    replacement = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);

    // step 4: Operate
    target = cJSON_GetArrayItem(arr, 1);
    cJSON_ReplaceItemViaPointer(arr, target, replacement);
    arr_size = cJSON_GetArraySize(arr);
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    val2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));

    // step 5: Validate
    validation = (int)(arr_size == 3) + (int)(val0 == 1.0) + (int)(val1 == 42.0) + (int)(val2 == 3.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(arr);
    (void)arr;
    (void)num0;
    (void)num1;
    (void)num2;
    (void)replacement;
    (void)target;
    (void)val0;
    (void)val1;
    (void)val2;
    (void)arr_size;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}