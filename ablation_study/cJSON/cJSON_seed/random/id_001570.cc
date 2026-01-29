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
//<ID> 1570
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *pi_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *dup_num = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    cJSON_bool added_arr_to_obj = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.5);
    num2 = cJSON_CreateNumber(-2.25);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "values", arr);
    pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    first_elem = cJSON_GetArrayItem(arr, 0);
    dup_num = cJSON_Duplicate(first_elem, false);
    dup_arr = cJSON_Duplicate(arr, true);
    cJSON_bool pi_is_number = cJSON_IsNumber(pi_item);

    // step 5: Validate
    validation_score = (int)(array_size == 2)
        + (int)added_arr_to_obj
        + (int)(first_elem != (cJSON *)0)
        + (int)(dup_num != (cJSON *)0)
        + (int)(dup_arr != (cJSON *)0)
        + (int)pi_is_number;

    // step 6: Cleanup
    cJSON_Delete(dup_num);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    (void)validation_score;
    (void)pi_is_number;
    (void)num1;
    (void)num2;
    (void)first_elem;
    (void)pi_item;
    // API sequence test completed successfully
    return 66;
}