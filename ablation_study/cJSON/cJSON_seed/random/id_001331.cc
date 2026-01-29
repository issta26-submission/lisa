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
//<ID> 1331
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
    cJSON *item_false = (cJSON *)0;
    cJSON *item_true = (cJSON *)0;
    cJSON_bool insert_ok1 = 0;
    cJSON_bool insert_ok2 = 0;
    cJSON_bool add_ok = 0;
    cJSON_bool detected_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two boolean items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_false = cJSON_CreateFalse();
    item_true = cJSON_CreateTrue();

    // step 3: Configure - insert booleans into the array and attach array to root
    insert_ok1 = cJSON_InsertItemInArray(arr, 0, item_false);
    insert_ok2 = cJSON_InsertItemInArray(arr, 1, item_true);
    add_ok = cJSON_AddItemToObject(root, "flags", arr);

    // step 4: Operate - inspect one of the items using cJSON_IsFalse
    detected_false = cJSON_IsFalse(item_false);

    // step 5: Validate - compute a simple validation score (no branches)
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)insert_ok1 + (int)insert_ok2 + (int)add_ok + (int)detected_false;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}