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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *detached_item = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    cJSON_bool detached_was_false = 0;
    int arr_size_after_detach = 0;

    // step 2: Setup / Configure - populate array and attach to root
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObjectCS(root, "items", arr);

    // step 3: Operate - detach the first element from the array and inspect it
    detached_item = cJSON_DetachItemFromArray(arr, 0);
    detached_was_false = cJSON_IsFalse(detached_item);
    arr_size_after_detach = cJSON_GetArraySize(arr);
    retrieved_items = cJSON_GetObjectItem(root, "items");

    // step 4: Validate / Integrate - reattach detached item under a new key and record state
    cJSON_AddItemToObjectCS(root, "moved_item", detached_item);
    cJSON_AddBoolToObject(root, "detached_was_false", detached_was_false);
    cJSON_AddNumberToObject(root, "remaining_count", (double)arr_size_after_detach);
    (void)retrieved_items; // used to satisfy meaningful data flow (retrieved pointer)

    // step 5: Cleanup - free all allocated cJSON structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}