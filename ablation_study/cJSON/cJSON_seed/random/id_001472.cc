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
//<ID> 1472
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
    cJSON *mult = (cJSON *)0;
    cJSON *scaled_sum = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_scaled_sum = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(2.0);
    n1 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);

    // step 3: Configure - attach array to root and add a multiplier number via AddNumberToObject
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    mult = cJSON_AddNumberToObject(root, "multiplier", 3.0);

    // step 4: Operate - read array items, compute sum, create scaled sum and add it to root, then remove multiplier
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    sum_val = cJSON_GetNumberValue(elem0) + cJSON_GetNumberValue(elem1);
    scaled_sum = cJSON_CreateNumber(sum_val * cJSON_GetNumberValue(mult));
    added_scaled_sum = cJSON_AddItemToObject(root, "scaled_sum", scaled_sum);
    cJSON_DeleteItemFromObject(root, "multiplier");

    // step 5: Validate - print unformatted JSON and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)added_arr + (int)added_scaled_sum + (int)cJSON_IsNumber(scaled_sum);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)elem0;
    (void)elem1;
    (void)mult;
    (void)n0;
    (void)n1;
    (void)added_arr;
    (void)added_scaled_sum;
    // API sequence test completed successfully
    return 66;
}