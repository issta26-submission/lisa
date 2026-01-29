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
//<ID> 1439
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
    cJSON *picked = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    char *printed = (char *)0;
    double picked_val = 0.0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Initialize - create root object and an array with two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);

    // step 3: Configure - attach the array to the root object
    added = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate - pick an item from the array, extract its numeric value, create a sum node, and print unformatted
    picked = cJSON_GetArrayItem(arr, 1);
    picked_val = cJSON_GetNumberValue(picked);
    sum_node = cJSON_CreateNumber(picked_val + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0)));
    cJSON_AddItemToObject(root, "sum", sum_node);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)(picked != (cJSON *)0) + (int)(printed != (char *)0) + (int)(added != 0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)picked_val;
    (void)num0;
    (void)num1;
    (void)added;
    // API sequence test completed successfully
    return 66;
}