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
//<ID> 809
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *ints_arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_num = 0;
    cJSON_bool added_false = 0;
    cJSON_bool is_array = 0;
    cJSON_bool is_false = 0;
    char *printed = (char *)0;
    int nums[3];
    int validation_score = 0;

    // step 2: Initialize
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    root = cJSON_CreateObject();
    ints_arr = cJSON_CreateIntArray((const int *)nums, 3);
    num_item = cJSON_CreateNumber(7.5);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "ints", ints_arr);
    added_num = cJSON_AddItemToObject(root, "value", num_item);
    added_false = cJSON_AddItemToObject(root, "flag", false_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "ints");
    retrieved_num = cJSON_GetObjectItem(root, "value");
    retrieved_false = cJSON_GetObjectItem(root, "flag");
    is_array = cJSON_IsArray(retrieved_arr);
    is_false = cJSON_IsFalse(retrieved_false);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_arr != (cJSON *)0) + (retrieved_num != (cJSON *)0) + (retrieved_false != (cJSON *)0) + (int)is_array + (int)is_false + (int)added_arr + (int)added_num + (int)added_false + (printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}