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
//<ID> 1461
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
    cJSON *n2 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    cJSON *str_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool added_flag = 0;

    // step 2: Initialize - create root object, an array and three numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(3.5);
    n1 = cJSON_CreateNumber(4.5);
    n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    added_flag = cJSON_AddItemToObject(root, "values", arr);

    // step 3: Configure - add a descriptive string to the root object using required API
    str_node = cJSON_AddStringToObject(root, "label", "cJSON-fuzz-test");

    // step 4: Operate - read array size, extract two elements and compute their numeric sum, attach sum to root
    arr_size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    sum_val = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(second);
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);

    // step 5: Validate - produce unformatted JSON and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + arr_size + (int)cJSON_IsNumber(sum_node) + (int)(str_node != (cJSON *)0) + (int)added_flag;

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)first;
    (void)second;
    (void)arr_size;
    (void)n0;
    (void)n1;
    (void)n2;
    (void)added_flag;
    (void)str_node;
    // API sequence test completed successfully
    return 66;
}