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
//<ID> 1332
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
    cJSON *false_item = (cJSON *)0;
    cJSON_bool insert_ok = 0;
    cJSON_bool add_ok = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();

    // step 3: Configure - insert the false item into the array
    insert_ok = cJSON_InsertItemInArray(arr, 0, false_item);

    // step 4: Operate - attach the array to the root object
    add_ok = cJSON_AddItemToObject(root, "data", arr);

    // step 5: Validate - verify the item is false and compute a small score
    is_false = cJSON_IsFalse(false_item);
    validation_score = (int)insert_ok + (int)add_ok + (int)is_false;
    (void)validation_score;

    // step 6: Cleanup - delete the root (which frees array and its items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}