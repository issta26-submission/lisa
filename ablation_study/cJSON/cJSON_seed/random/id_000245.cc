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
//<ID> 245
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
    cJSON *retrieved_arr_item0 = (cJSON *)0;
    cJSON *retrieved_arr_item1 = (cJSON *)0;
    cJSON *retrieved_obj_numbers = (cJSON *)0;
    cJSON *meta_obj = (cJSON *)0;
    cJSON *final_sum_item = (cJSON *)0;
    char *printed = (char *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    double sum = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");

    // step 3: Configure
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    meta_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta_obj);

    // step 4: Operate
    retrieved_arr_item0 = cJSON_GetArrayItem(arr, 0);
    retrieved_arr_item1 = cJSON_GetArrayItem(arr, 1);
    retrieved_obj_numbers = cJSON_GetObjectItem(root, "numbers");
    val0 = cJSON_GetNumberValue(retrieved_arr_item0);
    val1 = cJSON_GetNumberValue(retrieved_arr_item1);
    sum = val0 + val1;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 5: Validate
    final_sum_item = cJSON_GetObjectItem(root, "sum");
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}