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
//<ID> 992
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "numbers";
    const char *ans_key = "answer";
    const char *raw_key = "raw";
    const char *flag_key = "flag";
    double first_value = 1.0;
    double second_value = 2.0;
    double answer_value = 42.0;
    cJSON *root = (cJSON *)0;
    cJSON *num_array = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *answer_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON_bool add_array_ok1 = (cJSON_bool)0;
    cJSON_bool add_array_ok2 = (cJSON_bool)0;
    cJSON_bool add_obj_ok_arr = (cJSON_bool)0;
    cJSON_bool add_obj_ok_ans = (cJSON_bool)0;
    cJSON_bool add_obj_ok_raw = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    char *serialized = (char *)0;
    cJSON *fetched_answer = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an empty array
    root = cJSON_CreateObject();
    num_array = cJSON_CreateArray();
    add_obj_ok_arr = cJSON_AddItemToObject(root, arr_key, num_array);

    // step 3: Configure - create numbers, add them to array and add a number and other types to root
    num1 = cJSON_CreateNumber(first_value);
    num2 = cJSON_CreateNumber(second_value);
    add_array_ok1 = cJSON_AddItemToArray(num_array, num1);
    add_array_ok2 = cJSON_AddItemToArray(num_array, num2);
    answer_item = cJSON_CreateNumber(answer_value);
    add_obj_ok_ans = cJSON_AddItemToObject(root, ans_key, answer_item);
    flag_item = cJSON_AddBoolToObject(root, flag_key, (cJSON_bool)1);
    raw_item = cJSON_CreateRaw("raw_payload");
    add_obj_ok_raw = cJSON_AddItemToObject(root, raw_key, raw_item);
    is_raw = cJSON_IsRaw(raw_item);

    // step 4: Operate - serialize the constructed JSON
    serialized = cJSON_Print(root);

    // step 5: Validate - retrieve items and compute a simple validation score
    fetched_answer = cJSON_GetObjectItem(root, ans_key);
    first_elem = cJSON_GetArrayItem(num_array, 0);
    validation_score = (int)(root != (cJSON *)0)
                     + (int)(num_array != (cJSON *)0)
                     + (int)(add_array_ok1)
                     + (int)(add_array_ok2)
                     + (int)(add_obj_ok_arr)
                     + (int)(add_obj_ok_ans)
                     + (int)(add_obj_ok_raw)
                     + (int)(flag_item != (cJSON *)0)
                     + (int)(is_raw)
                     + (int)(serialized != (char *)0)
                     + (int)(fetched_answer != (cJSON *)0 && cJSON_GetNumberValue(fetched_answer) == answer_value)
                     + (int)(first_elem != (cJSON *)0 && cJSON_GetNumberValue(first_elem) == first_value);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}