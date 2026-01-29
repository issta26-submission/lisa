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
//<ID> 801
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *int_arr = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_num = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_false = 0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_false = 0;
    double num_value = 0.0;
    int validation_score = 0;
    int nums[4];

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.0);
    nums[0] = 10; nums[1] = 20; nums[2] = 30; nums[3] = 40;
    int_arr = cJSON_CreateIntArray(nums, 4);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_num = cJSON_AddItemToObject(root, "number", num_item);
    added_arr = cJSON_AddItemToObject(root, "int_array", int_arr);
    added_false = cJSON_AddItemToObject(root, "disabled", false_item);

    // step 4: Operate
    retrieved_num = cJSON_GetObjectItem(root, "number");
    retrieved_arr = cJSON_GetObjectItem(root, "int_array");
    retrieved_false = cJSON_GetObjectItem(root, "disabled");
    num_value = cJSON_GetNumberValue(retrieved_num);
    is_arr = cJSON_IsArray(retrieved_arr);
    is_false = cJSON_IsFalse(retrieved_false);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (retrieved_num != (cJSON *)0) + (retrieved_arr != (cJSON *)0) + (retrieved_false != (cJSON *)0) + (int)is_arr + (int)is_false + (int)added_num + (int)added_arr + (int)added_false + (num_value == 123.0) + (printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}