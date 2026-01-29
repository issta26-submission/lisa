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
//<ID> 1473
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
    cJSON *individual_ptr = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added_array = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);

    // step 3: Configure - attach array to root and add an individual number via AddNumberToObject
    added_array = cJSON_AddItemToObject(root, "values", arr);
    individual_ptr = cJSON_AddNumberToObject(root, "individual", 5.0);

    // step 4: Operate - retrieve array items and compute a sum including the individual number
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    sum_val = cJSON_GetNumberValue(elem0) + cJSON_GetNumberValue(elem1) + cJSON_GetNumberValue(individual_ptr);

    // step 5: Validate - add computed sum into the root object and remove the individual item, then print
    sum_node = cJSON_AddNumberToObject(root, "sum", sum_val);
    cJSON_DeleteItemFromObject(root, "individual");
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)added_array + (int)cJSON_IsNumber(sum_node);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)elem0;
    (void)elem1;
    (void)n0;
    (void)n1;
    (void)individual_ptr;
    (void)added_array;
    // API sequence test completed successfully
    return 66;
}