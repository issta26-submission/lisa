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
//<ID> 1327
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(3.0);
    n1 = cJSON_CreateNumber(4.0);

    // step 3: Configure - populate the array and attach it to the root object
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate - retrieve array, extract elements and compute a sum, then add sum back to root
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    elem1 = cJSON_GetArrayItem(retrieved_arr, 1);
    v0 = cJSON_GetNumberValue(elem0);
    v1 = cJSON_GetNumberValue(elem1);
    sum = v0 + v1;
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(sum_item != (cJSON *)0) + (int)(sum == 7.0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}