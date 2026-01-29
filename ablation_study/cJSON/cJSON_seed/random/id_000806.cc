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
//<ID> 806
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
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_array = 0;
    cJSON_bool is_false = 0;
    int arr_size = 0;
    double num_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int nums[4];
    nums[0] = 1;
    nums[1] = 2;
    nums[2] = 3;
    nums[3] = 4;
    int_arr = cJSON_CreateIntArray(nums, 4);
    num_item = cJSON_CreateNumber(7.5);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "ints", int_arr);
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "ints");
    retrieved_num = cJSON_GetObjectItem(root, "value");
    retrieved_false = cJSON_GetObjectItem(root, "flag");
    is_array = cJSON_IsArray(retrieved_arr);
    is_false = cJSON_IsFalse(retrieved_false);
    arr_size = cJSON_GetArraySize(retrieved_arr);
    num_val = cJSON_GetNumberValue(retrieved_num);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)is_array + (int)is_false + (arr_size == 4) + (num_val > 7.0) + (printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}