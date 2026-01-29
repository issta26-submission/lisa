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
//<ID> 608
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *int_arr = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *expected_num = (cJSON *)0;
    int numbers[3] = {10, 20, 30};
    double first_val = 0.0;
    cJSON_bool cmp_res = 0;

    // step 2: Initialize
    int_arr = cJSON_CreateIntArray(numbers, 3);
    root = cJSON_CreateObject();
    expected_num = cJSON_CreateNumber(20.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", int_arr);

    // step 4: Operate
    arr_ref = cJSON_GetObjectItem(root, "numbers");
    first_item = cJSON_GetArrayItem(arr_ref, 0);
    first_val = cJSON_GetNumberValue(first_item);
    detached_item = cJSON_DetachItemFromArray(arr_ref, 1);
    cmp_res = cJSON_Compare(detached_item, expected_num, 1);

    // step 5: Validate
    (void)first_val;
    (void)cmp_res;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_item);
    cJSON_Delete(expected_num);

    // API sequence test completed successfully
    return 66;
}