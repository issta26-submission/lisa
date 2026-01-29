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
//<ID> 1520
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *answer_item = (cJSON *)0;
    cJSON *retrieved_values = (cJSON *)0;
    cJSON *retrieved_answer = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_flag = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    answer_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    added_arr = cJSON_AddItemToObject(root, "values", arr);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_flag = cJSON_AddItemToObject(root, "flag", false_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_values = cJSON_GetObjectItem(root, "values");
    retrieved_answer = cJSON_GetObjectItem(root, "answer");
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    is_false = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)(retrieved_values != (cJSON *)0) + (int)(retrieved_answer != (cJSON *)0) + (int)(answer_item != (cJSON *)0) + (int)is_false + (int)added_arr + (int)added_num1 + (int)added_num2 + (int)added_flag;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_values;
    (void)retrieved_answer;
    (void)retrieved_flag;
    (void)answer_item;
    (void)num1;
    (void)num2;
    // API sequence test completed successfully
    return 66;
}