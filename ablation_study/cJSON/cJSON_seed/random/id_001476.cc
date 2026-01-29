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
//<ID> 1476
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
    cJSON *arr_elem0 = (cJSON *)0;
    cJSON *arr_elem1 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    cJSON *direct_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added_arr = 0;

    // step 2: Initialize - create root object and an array with two numbers
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(32.5);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);

    // step 3: Configure - attach array to root and add a direct numeric property using AddNumberToObject
    added_arr = cJSON_AddItemToObject(root, "list", arr);
    direct_node = cJSON_AddNumberToObject(root, "direct_value", 7.25);

    // step 4: Operate - retrieve array elements via cJSON_GetArrayItem, compute their sum and add result to root
    arr_elem0 = cJSON_GetArrayItem(arr, 0);
    arr_elem1 = cJSON_GetArrayItem(arr, 1);
    sum_val = cJSON_GetNumberValue(arr_elem0) + cJSON_GetNumberValue(arr_elem1) + cJSON_GetNumberValue(direct_node);
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);

    // step 5: Validate - remove the direct_value item and produce a printed representation as a simple validation
    cJSON_DeleteItemFromObject(root, "direct_value");
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)added_arr + (int)cJSON_IsNumber(sum_node);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)arr_elem0;
    (void)arr_elem1;
    (void)n0;
    (void)n1;
    (void)direct_node;
    (void)added_arr;
    // API sequence test completed successfully
    return 66;
}