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
//<ID> 1433
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
    cJSON *child = (cJSON *)0;
    cJSON *num_copy = (cJSON *)0;
    char *printed = (char *)0;
    double picked_value = 0.0;
    cJSON_bool add_result1 = 0;
    cJSON_bool add_result2 = 0;
    cJSON_bool add_result3 = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    add_result1 = cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate - pick the second array item and read its numeric value
    picked = cJSON_GetArrayItem(arr, 1);
    picked_value = cJSON_GetNumberValue(picked);

    // step 4: Configure - create a child object summarizing the picked value and attach to root
    child = cJSON_CreateObject();
    num_copy = cJSON_CreateNumber(picked_value);
    add_result2 = cJSON_AddItemToObject(child, "picked", num_copy);
    add_result3 = cJSON_AddItemToObject(root, "summary", child);

    // step 5: Print - produce an unformatted JSON string representation
    printed = cJSON_PrintUnformatted(root);

    // step 6: Cleanup - compute a small validation score, free printed buffer and delete the JSON tree
    validation_score = (int)(printed != (char *)0) + (int)add_result1 + (int)add_result2 + (int)add_result3;
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    (void)validation_score;
    (void)num0;
    (void)num1;
    (void)picked;
    (void)picked_value;
    (void)num_copy;
    // API sequence test completed successfully
    return 66;
}