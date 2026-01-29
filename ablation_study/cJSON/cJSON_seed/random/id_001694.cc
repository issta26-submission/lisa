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
//<ID> 1694
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
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    cJSON *replaced = (cJSON *)0;
    char *printed = (char *)0;
    int size_before = 0;
    int size_after = 0;
    double first_val = 0.0;
    double second_val = 0.0;
    double replaced_val = 0.0;
    cJSON_bool add_ok = 0;
    cJSON_bool replace_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.0);
    num2 = cJSON_CreateNumber(30.0);

    // step 3: Configure (build array and attach to root)
    add_ok = cJSON_AddItemToArray(arr, num0);
    add_ok = cJSON_AddItemToArray(arr, num1);
    add_ok = cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate (inspect values, then replace second element)
    size_before = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first);
    second = cJSON_GetArrayItem(arr, 1);
    second_val = cJSON_GetNumberValue(second);
    newnum = cJSON_CreateNumber(42.0);
    replace_ok = cJSON_ReplaceItemViaPointer(arr, second, newnum);
    size_after = cJSON_GetArraySize(arr);
    replaced = cJSON_GetArrayItem(arr, 1);
    replaced_val = cJSON_GetNumberValue(replaced);

    // step 5: Validate
    validation = (int)(size_before == 3) + (int)(size_after == 3) + (int)(first_val == 10.0) + (int)(second_val == 20.0) + (int)(replaced_val == 42.0) + (int)replace_ok;
    (void)validation;

    // step 6: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)num2;
    (void)newnum;
    (void)first;
    (void)second;
    (void)replaced;
    (void)first_val;
    (void)second_val;
    (void)replaced_val;
    (void)add_ok;
    (void)replace_ok;
    (void)size_before;
    (void)size_after;
    // API sequence test completed successfully
    return 66;
}