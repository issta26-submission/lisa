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
//<ID> 858
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
    cJSON *detached = (cJSON *)0;
    cJSON *new_holder = (cJSON *)0;
    cJSON_bool added_holder = 0;
    cJSON_bool added_moved = 0;
    cJSON_bool is_num = 0;
    int validation_score = 0;
    const char *json_text = "{\"arr\":[1,2,3],\"meta\":{\"id\":42}}";

    // step 2: Initialize (parse JSON)
    root = cJSON_Parse(json_text);
    arr = cJSON_GetObjectItem(root, "arr");

    // step 3: Configure (create a new holder object and attach to root)
    new_holder = cJSON_CreateObject();
    added_holder = cJSON_AddItemToObject(root, "moved_items", new_holder);

    // step 4: Operate (detach the second element from the array and inspect it)
    detached = cJSON_DetachItemFromArray(arr, 1);
    is_num = cJSON_IsNumber(detached);

    // step 5: Validate (move the detached item into the new holder and compute a score)
    added_moved = cJSON_AddItemToObject(new_holder, "item_1", detached);
    validation_score = (root != (cJSON *)0) + (arr != (cJSON *)0) + (detached != (cJSON *)0) + (int)is_num + (int)added_holder + (int)added_moved;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}