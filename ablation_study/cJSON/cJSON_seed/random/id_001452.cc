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
//<ID> 1452
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
    cJSON *num2 = (cJSON *)0;
    cJSON *old_answer = (cJSON *)0;
    cJSON *new_answer = (cJSON *)0;
    cJSON *fetched_values = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Initialize - create root object and an array with two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);

    // step 3: Configure - attach the array to the root and add an "answer" numeric member
    added = cJSON_AddItemToObject(root, "values", arr);
    old_answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", old_answer);

    // step 4: Operate - retrieve the "answer" item, replace it via pointer, fetch the array and append another number
    fetched_values = cJSON_GetObjectItem(root, "answer");
    new_answer = cJSON_CreateNumber(84.0);
    cJSON_ReplaceItemViaPointer(root, fetched_values, new_answer);
    fetched_values = cJSON_GetObjectItem(root, "values");
    num2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(fetched_values, num2);

    // step 5: Validate - compute a sum from the array, attach it, and render the JSON
    sum_val = cJSON_GetNumberValue(cJSON_GetArrayItem(fetched_values, 0)) + cJSON_GetNumberValue(cJSON_GetArrayItem(fetched_values, 1)) + cJSON_GetNumberValue(cJSON_GetArrayItem(fetched_values, 2));
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)(added != 0) + (int)cJSON_IsNumber(new_answer);

    // step 6: Cleanup - free printed buffer and delete the whole JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)num0;
    (void)num1;
    (void)num2;
    (void)old_answer;
    (void)new_answer;
    (void)fetched_values;
    (void)added;
    // API sequence test completed successfully
    return 66;
}