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
//<ID> 1469
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
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    cJSON *str_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_sum = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(3.5);
    n1 = cJSON_CreateNumber(4.5);

    // step 3: Configure - populate array, attach to root, and add a descriptive string
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    str_node = cJSON_AddStringToObject(root, "description", "two numbers");

    // step 4: Operate - inspect array, compute sum using number values, and attach sum
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    arr_size = cJSON_GetArraySize(arr);
    sum_val = cJSON_GetNumberValue(elem0) + cJSON_GetNumberValue(elem1);
    sum_node = cJSON_CreateNumber(sum_val);
    added_sum = cJSON_AddItemToObject(root, "sum", sum_node);

    // step 5: Validate - print unformatted JSON and derive a simple validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + arr_size + (int)(str_node != (cJSON *)0) + (int)added_arr + (int)added_sum + (int)cJSON_IsNumber(sum_node);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)elem0;
    (void)elem1;
    (void)arr_size;
    (void)added_arr;
    (void)added_sum;
    (void)n0;
    (void)n1;
    // API sequence test completed successfully
    return 66;
}