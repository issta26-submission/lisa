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
//<ID> 842
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    char *printed = (char *)0;
    double value1 = 0.0;
    double value2 = 0.0;
    cJSON_bool added_str1 = 0;
    cJSON_bool added_str2 = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_arr_to_root = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("hello");
    str2 = cJSON_CreateString("world");
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.71);

    // step 3: Configure
    added_str1 = cJSON_AddItemToArray(arr, str1);
    added_str2 = cJSON_AddItemToArray(arr, str2);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_arr_to_root = cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "data");
    value1 = cJSON_GetNumberValue(retrieved_arr->child->next->next);
    value2 = cJSON_GetNumberValue(retrieved_arr->child->next->next->next);
    printed = cJSON_Print(root);

    // step 5: Validate
    validation_score = (int)added_str1 + (int)added_str2 + (int)added_num1 + (int)added_num2 + (int)added_arr_to_root + (retrieved_arr != (cJSON *)0) + (printed != (char *)0) + (value1 > 0.0) + (value2 > 0.0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}