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
//<ID> 1691
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    double val_first = 0.0;
    double val_second = 0.0;
    cJSON_bool add_result = 0;
    cJSON_bool replace_result = 0;
    int validation = 0;

    // step 2: Initialize
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.0);
    n1 = cJSON_CreateNumber(2.0);
    n2 = cJSON_CreateNumber(3.0);

    // step 3: Configure
    add_result = cJSON_AddItemToArray(arr, n0);
    add_result = cJSON_AddItemToArray(arr, n1);
    add_result = cJSON_AddItemToArray(arr, n2);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    second = cJSON_GetArrayItem(arr, 1);
    replacement = cJSON_CreateNumber(42.0);
    replace_result = cJSON_ReplaceItemViaPointer(arr, second, replacement);
    size_after = cJSON_GetArraySize(arr);
    val_first = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    val_second = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));

    // step 5: Validate
    validation = (int)(size_before == 3) + (int)(size_after == 3) + (int)(val_first == 1.0) + (int)(val_second == 42.0) + (int)replace_result + (int)add_result;
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(arr);
    (void)arr;
    (void)n0;
    (void)n1;
    (void)n2;
    (void)second;
    (void)replacement;
    (void)size_before;
    (void)size_after;
    (void)val_first;
    (void)val_second;
    (void)add_result;
    (void)replace_result;
    // API sequence test completed successfully
    return 66;
}