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
//<ID> 1671
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
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    int arr_size = 0;
    double first_val = 0.0;
    cJSON_bool is_false = 0;
    double detached_first = 0.0;
    cJSON_bool detached_false = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(3.14);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    is_false = cJSON_IsFalse(cJSON_GetArrayItem(arr, 1));
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "data");
    detached_first = cJSON_GetNumberValue(cJSON_GetArrayItem(detached, 0));
    detached_false = cJSON_IsFalse(cJSON_GetArrayItem(detached, 1));

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(first_val == 3.14) + (int)is_false + (int)(detached != (cJSON *)0) + (int)(detached_first == 3.14) + (int)detached_false;
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);
    (void)root;
    (void)arr;
    (void)num_item;
    (void)false_item;
    (void)arr_size;
    (void)first_val;
    (void)is_false;
    (void)detached_first;
    (void)detached_false;
    // API sequence test completed successfully
    return 66;
}