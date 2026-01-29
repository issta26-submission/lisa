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
//<ID> 1448
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
    cJSON *label_node = (cJSON *)0;
    cJSON *picked0 = (cJSON *)0;
    cJSON *picked1 = (cJSON *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_root_arr = 0;
    cJSON_bool added_sum = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(5.5);
    added_arr = cJSON_AddItemToArray(arr, num0);
    added_arr = (cJSON_bool)(added_arr + cJSON_AddItemToArray(arr, num1));

    // step 3: Configure - attach the array to the root and add a descriptive string
    added_root_arr = cJSON_AddItemToObject(root, "numbers", arr);
    label_node = cJSON_AddStringToObject(root, "label", "example_set");

    // step 4: Operate - pick items, compute sum, create a sum node and attach it to root
    picked0 = cJSON_GetArrayItem(arr, 0);
    picked1 = cJSON_GetArrayItem(arr, 1);
    sum_node = cJSON_CreateNumber(cJSON_GetNumberValue(picked0) + cJSON_GetNumberValue(picked1));
    added_sum = cJSON_AddItemToObject(root, "sum", sum_node);

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(picked0 != (cJSON *)0) + (int)(picked1 != (cJSON *)0) + (int)(label_node != (cJSON *)0) + (int)(added_root_arr != 0) + (int)(added_sum != 0);

    // step 6: Cleanup - delete the whole JSON tree
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)added_arr;
    return 66;
}