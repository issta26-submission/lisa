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
//<ID> 1449
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    cJSON *str_node = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.0);

    // step 3: Configure - add numbers to array, attach array to root, and add a string property
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added = cJSON_AddItemToObject(root, "values", arr);
    str_node = cJSON_AddStringToObject(root, "label", "example");

    // step 4: Operate - compute sum from array elements, create a sum node and attach it, then print
    picked = cJSON_GetArrayItem(arr, 0);
    sum_val = cJSON_GetNumberValue(picked) + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)(printed != (char *)0) + (int)(str_node != (cJSON *)0) + (int)(added != 0) + (int)cJSON_IsNumber(sum_node);

    // step 6: Cleanup - free printed buffer and delete the whole JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)num0;
    (void)num1;
    (void)picked;
    (void)sum_val;
    (void)added;

    // API sequence test completed successfully
    return 66;
}