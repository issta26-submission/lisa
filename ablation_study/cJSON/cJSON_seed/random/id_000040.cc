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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item_true = cJSON_CreateTrue();
    cJSON *item_false = cJSON_CreateFalse();
    cJSON *detached_item = (cJSON *)0;
    cJSON *lookup = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool was_false = 0;

    // step 2: Setup - populate array and attach to root
    cJSON_AddItemToArray(arr, item_true);
    cJSON_AddItemToArray(arr, item_false);
    cJSON_AddItemToObjectCS(root, "items", arr);

    // step 3: Core operations - detach an element from the array
    detached_item = cJSON_DetachItemFromArray(arr, 1);

    // step 4: Configure - reattach detached item into root as a named member
    cJSON_AddItemToObjectCS(root, "detached", detached_item);

    // step 5: Validate - inspect array size and the detached item's boolean state
    arr_size = cJSON_GetArraySize(arr);
    lookup = cJSON_GetObjectItem(root, "detached");
    was_false = cJSON_IsFalse(lookup);
    cJSON_AddNumberToObject(root, "remaining", (double)arr_size);
    cJSON_AddBoolToObject(root, "detached_was_false", was_false);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}