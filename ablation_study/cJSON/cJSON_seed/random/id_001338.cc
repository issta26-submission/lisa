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
//<ID> 1338
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON_bool is_false = 0;
    cJSON_bool insert_ret0 = 0;
    cJSON_bool insert_ret1 = 0;
    cJSON_bool add_ret_arr = 0;
    cJSON_bool add_ret_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array, a false flag and two numbers
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);

    // step 3: Configure - insert numbers into array and add array and flag into root object
    insert_ret0 = cJSON_InsertItemInArray(arr, 0, num1);
    insert_ret1 = cJSON_InsertItemInArray(arr, 1, num2);
    add_ret_arr = cJSON_AddItemToObject(root, "numbers", arr);
    add_ret_flag = cJSON_AddItemToObject(root, "flag", false_item);

    // step 4: Operate - retrieve elements and evaluate the false flag
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    elem1 = cJSON_GetArrayItem(retrieved_arr, 1);
    is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));

    // step 5: Validate - compute a simple validation score from outcomes
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(elem0 != (cJSON *)0) + (int)(elem1 != (cJSON *)0) + (int)(is_false != 0) + (int)(insert_ret0 != 0) + (int)(insert_ret1 != 0) + (int)(add_ret_arr != 0) + (int)(add_ret_flag != 0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}