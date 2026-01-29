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
//<ID> 1698
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
    cJSON *replacement = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    int size_after = 0;
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
    replacement = cJSON_CreateNumber(42.0);
    replace_result = cJSON_ReplaceItemViaPointer(arr, num1, replacement);
    size_after = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetNumberValue(item0);
    val1 = cJSON_GetNumberValue(item1);

    // step 5: Validate
    validation = (int)(size_after == 3) + (int)cJSON_IsNumber(item0) + (int)cJSON_IsNumber(item1) + (int)(val0 == 1.0) + (int)(val1 == 42.0) + (int)replace_result;
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)num2;
    (void)replacement;
    (void)item0;
    (void)item1;
    (void)val0;
    (void)val1;
    (void)size_after;
    (void)replace_result;
    // API sequence test completed successfully
    return 66;
}