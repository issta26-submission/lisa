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
//<ID> 802
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *container = (cJSON *)0;
    cJSON *int_array = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_container = (cJSON *)0;
    cJSON *retrieved_int_array = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_number = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;
    double number_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    container = cJSON_CreateObject();
    {
        int nums[4];
        nums[0] = 10;
        nums[1] = 20;
        nums[2] = 30;
        nums[3] = 40;
        int_array = cJSON_CreateIntArray(nums, 4);
    }
    num_item = cJSON_CreateNumber(7.25);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "container", container);
    cJSON_AddItemToObject(container, "ints", int_array);
    cJSON_AddItemToObject(container, "pi_like", num_item);
    cJSON_AddItemToObject(container, "disabled", false_item);

    // step 4: Operate
    retrieved_container = cJSON_GetObjectItem(root, "container");
    retrieved_int_array = cJSON_GetObjectItemCaseSensitive(retrieved_container, "ints");
    is_arr = cJSON_IsArray(retrieved_int_array);
    retrieved_flag = cJSON_GetObjectItemCaseSensitive(retrieved_container, "disabled");
    is_false = cJSON_IsFalse(retrieved_flag);
    retrieved_number = cJSON_GetObjectItemCaseSensitive(retrieved_container, "pi_like");
    number_value = cJSON_GetNumberValue(retrieved_number);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_container != (cJSON *)0) + (retrieved_int_array != (cJSON *)0) + (int)is_arr + (retrieved_flag != (cJSON *)0) + (int)is_false + (retrieved_number != (cJSON *)0) + (printed != (char *)0) + (number_value > 7.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}