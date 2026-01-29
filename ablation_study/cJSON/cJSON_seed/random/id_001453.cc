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
//<ID> 1453
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *obj_values = (cJSON *)0;
    cJSON *picked_before = (cJSON *)0;
    cJSON *picked_after = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added = 0;
    cJSON_bool replaced = 0;

    // Step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.5);
    num1 = cJSON_CreateNumber(2.5);

    // Step 3: Configure - add numbers to array and attach the array to the root object
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added = cJSON_AddItemToObject(root, "values", arr);

    // Step 4: Operate - retrieve the "values" object, replace the second array element via pointer, then compute a sum and attach it
    obj_values = cJSON_GetObjectItem(root, "values");
    picked_before = cJSON_GetArrayItem(arr, 1);
    replacement = cJSON_CreateNumber(42.0);
    replaced = cJSON_ReplaceItemViaPointer(arr, num1, replacement);
    picked_after = cJSON_GetArrayItem(arr, 1);
    sum_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0)) + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);
    printed = cJSON_PrintUnformatted(root);

    // Step 5: Validate - derive a small validation score from results
    validation_score = (int)(obj_values != (cJSON *)0) + (int)(picked_before != (cJSON *)0) + (int)(picked_after != (cJSON *)0) + (int)(printed != (char *)0) + (int)replaced + (int)cJSON_IsNumber(sum_node);

    // Step 6: Cleanup - free printed buffer and delete the whole JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)added;
    (void)num0;
    (void)num1;
    (void)replacement;
    (void)picked_before;
    (void)picked_after;
    // API sequence test completed successfully
    return 66;
}