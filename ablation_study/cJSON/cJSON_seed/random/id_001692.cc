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
//<ID> 1692
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
    cJSON *to_replace = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    int arr_size = 0;
    double replaced_value = 0.0;
    int validation = 0;

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
    arr_size = cJSON_GetArraySize(arr);
    to_replace = cJSON_GetArrayItem(arr, 1);
    replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemViaPointer(arr, to_replace, replacement);
    replaced_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));

    // step 5: Validate
    validation = (int)(arr_size == 3) + (int)(replaced_value == 42.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)num2;
    (void)to_replace;
    (void)replacement;
    (void)arr_size;
    (void)replaced_value;
    // API sequence test completed successfully
    return 66;
}