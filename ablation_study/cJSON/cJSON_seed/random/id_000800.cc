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
//<ID> 800
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *obj = (cJSON *)0;
    cJSON *int_arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retr_obj = (cJSON *)0;
    cJSON *retr_arr = (cJSON *)0;
    cJSON *retr_false = (cJSON *)0;
    cJSON_bool added_int = 0;
    cJSON_bool added_num = 0;
    cJSON_bool added_false = 0;
    cJSON_bool is_array = 0;
    cJSON_bool is_false = 0;
    char *printed = (char *)0;
    double retrieved_number = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    int nums[4] = {10, 20, 30, 40};
    root = cJSON_CreateObject();
    obj = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(nums, 4);
    num_item = cJSON_CreateNumber(7.77);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_int = cJSON_AddItemToObject(obj, "ints", int_arr);
    added_num = cJSON_AddItemToObject(obj, "lucky", num_item);
    added_false = cJSON_AddItemToObject(obj, "flag", false_item);
    cJSON_AddItemToObject(root, "container", obj);

    // step 4: Operate
    retr_obj = cJSON_GetObjectItem(root, "container");
    retr_arr = cJSON_GetObjectItemCaseSensitive(retr_obj, "ints");
    retr_false = cJSON_GetObjectItemCaseSensitive(retr_obj, "flag");
    is_array = cJSON_IsArray(retr_arr);
    is_false = cJSON_IsFalse(retr_false);
    retrieved_number = cJSON_GetNumberValue(cJSON_GetObjectItem(retr_obj, "lucky"));

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retr_obj != (cJSON *)0) + (retr_arr != (cJSON *)0) + (int)is_array + (int)is_false + (printed != (char *)0) + (int)added_int + (int)added_num + (int)added_false + (retrieved_number == 7.77);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}