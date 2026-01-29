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
//<ID> 1394
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
    cJSON *picked = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *added_num = (cJSON *)0;
    double picked_val = 0.0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create object, array and numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(42.0);
    n2 = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Operate - get an array item, detach it, read its value and add it back as a named number
    picked = cJSON_GetArrayItem(arr, 0);
    detached = cJSON_DetachItemViaPointer(arr, picked);
    picked_val = cJSON_GetNumberValue(detached);
    added_num = cJSON_AddNumberToObject(root, "detached_value", picked_val);

    // step 4: Validate - gather array size and form a simple validation score
    arr_size = cJSON_GetArraySize(arr);
    validation_score = arr_size + (int)(added_num != (cJSON *)0) + (int)(detached != (cJSON *)0);

    // step 5: Cleanup - free detached item and the remaining tree
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    (void)validation_score;
    (void)picked;
    (void)picked_val;
    return 66;
}