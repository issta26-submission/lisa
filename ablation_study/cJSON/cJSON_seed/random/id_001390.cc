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
//<ID> 1390
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
    cJSON *n2 = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *added_num = (cJSON *)0;
    double picked_val = 0.0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create an object and populate an array of numbers
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.5);
    n1 = cJSON_CreateNumber(20.25);
    n2 = cJSON_CreateNumber(30.75);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Operate - pick an element, detach it from the array, and add its numeric value back to root
    picked = cJSON_GetArrayItem(arr, 1);
    picked_val = cJSON_GetNumberValue(picked);
    detached = cJSON_DetachItemViaPointer(arr, picked);
    added_num = cJSON_AddNumberToObject(root, "detached_value", picked_val);

    // step 4: Validate - compute array size and a simple validation score using obtained results
    arr_size = cJSON_GetArraySize(arr);
    validation_score = arr_size + (int)(added_num != (cJSON *)0) + (int)(detached != (cJSON *)0);

    // step 5: Cleanup - free detached item and the remaining cJSON tree
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)arr_size;
    (void)picked_val;
    (void)picked;
    return 66;
}