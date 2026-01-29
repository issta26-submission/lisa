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
//<ID> 1396
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
    cJSON *picked = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double picked_val = 0.0;
    int arr_size = 0;
    int success_score = 0;

    // step 2: Initialize - create object, array and numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.1);
    n2 = cJSON_CreateNumber(2.2);
    n3 = cJSON_CreateNumber(3.3);

    // step 3: Configure - populate array and attach to root
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate - pick an element, detach it by pointer, read its value and add a named number to root
    picked = cJSON_GetArrayItem(arr, 1);
    detached = cJSON_DetachItemViaPointer(arr, picked);
    picked_val = cJSON_GetNumberValue(detached);
    cJSON_AddNumberToObject(root, "picked_value", picked_val);

    // step 5: Validate - compute simple metrics based on current structure
    arr_size = cJSON_GetArraySize(arr);
    success_score = (int)(root != (cJSON *)0) + arr_size + (int)(detached != (cJSON *)0) + (int)(picked != (cJSON *)0);

    // step 6: Cleanup - free detached node and the whole tree
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)success_score;
    (void)arr_size;
    (void)picked_val;
    (void)picked;
    return 66;
}