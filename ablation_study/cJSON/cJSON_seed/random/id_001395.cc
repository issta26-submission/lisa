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
//<ID> 1395
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double picked_val = 0.0;
    int arr_size_before = 0;
    int arr_size_after = 0;
    int validation_score = 0;

    // step 2: Initialize - create a root object, an array and numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.5);
    n3 = cJSON_CreateNumber(30.25);

    // step 3: Configure - populate array and attach it to root
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate - retrieve the second element, detach it, read its value and re-add as named number
    found_arr = cJSON_GetObjectItem(root, "values");
    arr_size_before = cJSON_GetArraySize(found_arr);
    picked = cJSON_GetArrayItem(found_arr, 1);
    detached = cJSON_DetachItemViaPointer(found_arr, picked);
    picked_val = (detached != (cJSON *)0) ? cJSON_GetNumberValue(detached) : 0.0;
    cJSON_AddNumberToObject(root, "picked", picked_val);
    cJSON_Delete(detached);

    // step 5: Validate - obtain sizes and compute a simple validation score
    arr_size_after = cJSON_GetArraySize(found_arr);
    validation_score = (int)(root != (cJSON *)0) + arr_size_before + arr_size_after;

    // step 6: Cleanup - free the constructed cJSON tree
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)arr_size_before;
    (void)arr_size_after;
    (void)validation_score;
    (void)picked;
    (void)n1;
    (void)n2;
    (void)n3;
    return 66;
}