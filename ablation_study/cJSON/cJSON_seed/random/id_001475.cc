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
//<ID> 1475
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
    cJSON *mult_node = (cJSON *)0;
    cJSON *factor_node = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added_values = 0;
    cJSON_bool added_sum = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(2.0);
    n1 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);

    // step 3: Configure - attach array to root and add then remove a temporary number, then add a factor
    added_values = cJSON_AddItemToObject(root, "values", arr);
    mult_node = cJSON_AddNumberToObject(root, "multiplier", 10.0); /* will be removed */
    cJSON_DeleteItemFromObject(root, "multiplier");
    factor_node = cJSON_AddNumberToObject(root, "factor", 3.0);

    // step 4: Operate - retrieve array elements, compute a sum using the factor, create a sum node and add it
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    sum_val = cJSON_GetNumberValue(elem0) + cJSON_GetNumberValue(elem1) + cJSON_GetNumberValue(factor_node);
    sum_node = cJSON_CreateNumber(sum_val);
    added_sum = cJSON_AddItemToObject(root, "sum", sum_node);

    // step 5: Validate - produce a printed JSON and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)added_values + (int)added_sum + (int)cJSON_IsNumber(sum_node);
    cJSON_free((void *)printed);

    // step 6: Cleanup - delete the JSON tree and tidy up local references
    cJSON_Delete(root);
    (void)validation_score;
    (void)mult_node;
    (void)n0;
    (void)n1;
    (void)elem0;
    (void)elem1;
    (void)factor_node;
    (void)sum_val;
    (void)added_values;
    (void)added_sum;
    // API sequence test completed successfully
    return 66;
}