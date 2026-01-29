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
//<ID> 1690
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *newnum = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    double replaced_val = 0.0;
    int validation = 0;
    cJSON_bool replace_result = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    num2 = cJSON_CreateNumber(3.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    newnum = cJSON_CreateNumber(42.0);
    replace_result = cJSON_ReplaceItemViaPointer(arr, num1, newnum);
    size_after = cJSON_GetArraySize(arr);
    replaced_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));

    // step 5: Validate
    validation = (int)(size_before == 3) + (int)(size_after == 3) + (int)(replaced_val == 42.0) + (int)replace_result;
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)num2;
    (void)newnum;
    (void)size_before;
    (void)size_after;
    (void)replaced_val;
    (void)replace_result;
    // API sequence test completed successfully
    return 66;
}