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
//<ID> 629
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
    cJSON *false_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added_false = 0;
    cJSON_bool added_num = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool is_false = 0;
    int ptr_equal = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    num_item = cJSON_CreateNumber(123.0);

    // step 3: Configure
    added_false = cJSON_AddItemToArray(arr, false_item);
    added_num = cJSON_AddItemToArray(arr, num_item);
    added_arr_to_obj = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 0);
    is_false = cJSON_IsFalse(retrieved);

    // step 5: Validate
    ptr_equal = (retrieved == false_item);
    (void)added_false;
    (void)added_num;
    (void)added_arr_to_obj;
    (void)is_false;
    (void)ptr_equal;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}