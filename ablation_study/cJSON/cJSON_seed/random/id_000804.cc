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
//<ID> 804
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
    cJSON *num_item = (cJSON *)0;
    cJSON *int_arr = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_root = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_nums = 0;
    cJSON_bool added_num = 0;
    cJSON_bool added_false = 0;
    cJSON_bool added_obj = 0;
    cJSON_bool is_array = 0;
    cJSON_bool is_false = 0;
    double num_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    obj = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.0);
    int numbers[3];
    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;
    int_arr = cJSON_CreateIntArray(numbers, 3);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_nums = cJSON_AddItemToObject(obj, "numbers", int_arr);
    added_num = cJSON_AddItemToObject(obj, "magic", num_item);
    added_false = cJSON_AddItemToObject(obj, "enabled", false_item);
    added_obj = cJSON_AddItemToObject(root, "payload", obj);

    // step 4: Operate
    retrieved_root = cJSON_GetObjectItem(root, "payload");
    retrieved_arr = cJSON_GetObjectItem(retrieved_root, "numbers");
    retrieved_num = cJSON_GetObjectItem(retrieved_root, "magic");
    retrieved_false = cJSON_GetObjectItem(retrieved_root, "enabled");
    is_array = cJSON_IsArray(retrieved_arr);
    is_false = cJSON_IsFalse(retrieved_false);
    num_value = cJSON_GetNumberValue(retrieved_num);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (retrieved_root != (cJSON *)0) + (retrieved_arr != (cJSON *)0) + (retrieved_num != (cJSON *)0) + (retrieved_false != (cJSON *)0) + (int)is_array + (int)is_false + (int)added_nums + (int)added_num + (int)added_false + (int)added_obj + (printed != (char *)0) + (num_value == 123.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}