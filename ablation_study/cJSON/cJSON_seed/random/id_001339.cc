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
//<ID> 1339
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
    cJSON *fitem = (cJSON *)0;
    cJSON *titem = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON_bool is_false0 = 0;
    int validation_score = 0;

    // step 2: Initialize - create a root object and an array with boolean items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    fitem = cJSON_CreateFalse();
    titem = cJSON_CreateTrue();

    // step 3: Configure - insert boolean items into the array and attach the array to the root object
    cJSON_InsertItemInArray(arr, 0, fitem);
    cJSON_InsertItemInArray(arr, 1, titem);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - retrieve array elements and evaluate boolean state
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    is_false0 = cJSON_IsFalse(elem0);

    // step 5: Validate - compute a simple validation score from observed values
    validation_score = (int)is_false0 + (int)(cJSON_GetArraySize(arr) == 2) + (int)(cJSON_IsFalse(elem1) == 0);

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)elem0;
    (void)elem1;
    return 66;
}