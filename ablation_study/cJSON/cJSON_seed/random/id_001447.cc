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
//<ID> 1447
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
    cJSON *sum = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_arr = 0;
    cJSON *added_str = (cJSON *)0;
    int validation_score = 0;
    double sum_val = 0.0;

    // step 2: Initialize - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create numeric items and add to array
    n0 = cJSON_CreateNumber(1.618);
    n1 = cJSON_CreateNumber(2.718);
    added0 = cJSON_AddItemToArray(arr, n0);
    added1 = cJSON_AddItemToArray(arr, n1);
    added_arr = cJSON_AddItemToObject(root, "values", arr);
    added_str = cJSON_AddStringToObject(root, "label", "constants");

    // step 4: Operate - compute sum from array elements and attach to root
    sum_val = cJSON_GetNumberValue(n0) + cJSON_GetNumberValue(n1);
    sum = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum);

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)(cJSON_GetArraySize(arr) == 2) + (int)(cJSON_GetNumberValue(sum) > 0.0) + (int)(added_str != 0);

    // step 6: Cleanup - free the whole JSON tree
    cJSON_Delete(root);
    (void)validation_score;
    (void)added0;
    (void)added1;
    (void)added_arr;
    (void)sum_val;
    (void)added_str;

    // API sequence test completed successfully
    return 66;
}