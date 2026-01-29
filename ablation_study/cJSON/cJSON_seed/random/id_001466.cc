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
//<ID> 1466
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
    cJSON *picked0 = (cJSON *)0;
    cJSON *picked1 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, array, and numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(3.5);
    n1 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);

    // step 3: Configure - attach array to root and add a descriptive string property
    cJSON_AddItemToObject(root, "numbers", arr);
    label = cJSON_AddStringToObject(root, "description", "two numbers");

    // step 4: Operate - read array items, compute sum, add sum to root, and get array size
    picked0 = cJSON_GetArrayItem(arr, 0);
    picked1 = cJSON_GetArrayItem(arr, 1);
    sum_val = cJSON_GetNumberValue(picked0) + cJSON_GetNumberValue(picked1);
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - produce unformatted JSON and derive a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)cJSON_IsNumber(sum_node) + arr_size;

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)label;
    (void)picked0;
    (void)picked1;
    (void)sum_val;
    (void)arr_size;
    // API sequence test completed successfully
    return 66;
}