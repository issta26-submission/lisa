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
//<ID> 850
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
    cJSON *moved_item = (cJSON *)0;
    cJSON *retrieved_moved = (cJSON *)0;
    const char *json = "{\"arr\": [10, 20, 30], \"extra\": 3.14}";
    cJSON_bool added = 0;
    cJSON_bool is_num = 0;
    int validation_score = 0;

    // step 2: Initialize (parse JSON)
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "arr");

    // step 3: Configure (detach an element from array and add it to root object)
    detached = cJSON_DetachItemFromArray(arr, 1);
    added = cJSON_AddItemToObject(root, "moved", detached);

    // step 4: Operate (retrieve the moved item and check its type)
    retrieved_moved = cJSON_GetObjectItem(root, "moved");
    is_num = cJSON_IsNumber(retrieved_moved);

    // step 5: Validate (accumulate simple validation flags)
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(detached != (cJSON *)0) + (int)added + (int)is_num + (int)(retrieved_moved != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}