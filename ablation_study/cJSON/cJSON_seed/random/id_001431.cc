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
//<ID> 1431
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
    double val0 = 0.0;
    double val1 = 0.0;
    double sum = 0.0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_sum = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.5);
    num1 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added_arr = cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate - retrieve an array item, read numeric values, create a summary number and attach it
    picked = cJSON_GetArrayItem((const cJSON *)arr, 1);
    val0 = cJSON_GetNumberValue((const cJSON *const)num0);
    val1 = cJSON_GetNumberValue((const cJSON *const)picked);
    sum = val0 + val1;
    sum_node = cJSON_CreateNumber(sum);
    added_sum = cJSON_AddItemToObject(root, "sum", sum_node);

    // step 4: Serialize - produce an unformatted JSON string
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 5: Validate - derive a small validation score from operations
    validation_score = (int)added_arr + (int)added_sum + (int)(printed != (char *)0) + (int)(sum == (val0 + val1));

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    (void)validation_score;
    (void)val0;
    (void)val1;
    (void)sum_node;
    (void)num0;
    (void)num1;
    (void)picked;
    // API sequence test completed successfully
    return 66;
}