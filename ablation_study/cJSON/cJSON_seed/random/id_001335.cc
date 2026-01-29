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
//<ID> 1335
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
    cJSON *item_true = (cJSON *)0;
    cJSON *item_false = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two boolean items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_true = cJSON_CreateTrue();
    item_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - add and insert items into the array
    cJSON_AddItemToArray(arr, item_true);
    cJSON_InsertItemInArray(arr, 0, item_false);

    // step 4: Operate - retrieve first element and check if it is false
    elem0 = cJSON_GetArrayItem(arr, 0);
    is_false = cJSON_IsFalse(elem0);

    // step 5: Validate - compute a simple validation score
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)is_false;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}