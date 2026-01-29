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
//<ID> 1399
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
    cJSON *item_a = (cJSON *)0;
    cJSON *item_b = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *num_node = (cJSON *)0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create a root object and populate an array with two strings
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_a = cJSON_CreateString("alpha");
    item_b = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_b);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - pick the second element from the array
    picked = cJSON_GetArrayItem(arr, 1);

    // step 4: Operate - detach the picked element via pointer and record the new array size as a number in root
    detached = cJSON_DetachItemViaPointer(arr, picked);
    arr_size = cJSON_GetArraySize(arr);
    num_node = cJSON_AddNumberToObject(root, "remaining_count", (double)arr_size);

    // step 5: Validate - compute a simple validation score from the operations
    validation_score = (int)(arr_size) + (int)(detached != (cJSON *)0) + (int)(num_node != (cJSON *)0);

    // step 6: Cleanup - delete detached item and the main tree (which contains the number node)
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)item_a;
    (void)item_b;
    (void)picked;
    return 66;
}