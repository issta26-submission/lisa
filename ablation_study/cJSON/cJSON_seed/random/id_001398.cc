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
//<ID> 1398
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
    cJSON *found_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create object, array, and numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.5);
    n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Operate - retrieve array items, compute sum and add as named number, detach an item and reattach it under a name
    found_arr = cJSON_GetObjectItem(root, "numbers");
    elem0 = cJSON_GetArrayItem(found_arr, 0);
    elem1 = cJSON_GetArrayItem(found_arr, 1);
    v0 = cJSON_GetNumberValue(elem0);
    v1 = cJSON_GetNumberValue(elem1);
    sum = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", sum);
    detached = cJSON_DetachItemViaPointer(found_arr, elem1);
    cJSON_AddItemToObject(root, "moved", detached);

    // step 4: Validate - compute simple validation metrics
    arr_size = cJSON_GetArraySize(found_arr);
    validation_score = arr_size + (int)(root != (cJSON *)0);

    // step 5: Cleanup - free all cJSON resources
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)arr;
    (void)elem0;
    (void)elem1;
    (void)v0;
    (void)v1;
    (void)sum;
    (void)validation_score;
    (void)arr_size;
    return 66;
}