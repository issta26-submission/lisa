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
//<ID> 1529
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
    cJSON *num_item1 = (cJSON *)0;
    cJSON *num_item2 = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_value = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item1 = cJSON_CreateNumber(3.14);
    num_item2 = cJSON_CreateNumber(42.0);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_num1 = cJSON_AddItemToArray(arr, num_item1);
    added_num2 = cJSON_AddItemToArray(arr, num_item2);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *added_value_ptr = cJSON_AddNumberToObject(root, "value", 7.5);
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 4: Operate
    retrieved_value = cJSON_GetObjectItem(root, "value");
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    is_false = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate
    validation_score = (int)added_num1 + (int)added_num2 + (int)(retrieved_value != (cJSON *)0) + (int)(retrieved_flag != (cJSON *)0) + (int)is_false + (int)(added_value_ptr != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_value;
    (void)retrieved_flag;
    (void)added_value_ptr;
    // API sequence test completed successfully
    return 66;
}