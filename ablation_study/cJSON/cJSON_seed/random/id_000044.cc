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
//<ID> 44
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
    cJSON *found_moved = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    int arr_size_before = 0;
    int arr_size_after = 0;
    cJSON_bool detached_was_false = 0;

    // step 2: Setup - populate array and attach to root, record initial size
    cJSON_AddItemToArray(arr, item_true);
    cJSON_AddItemToArray(arr, item_false);
    cJSON_AddItemToObjectCS(root, "items", arr);
    arr_size_before = cJSON_GetArraySize(arr);

    // step 3: Operate - detach the second element from the array
    detached_item = cJSON_DetachItemFromArray(arr, 1);

    // step 4: Validate - check detached item and new array size
    detached_was_false = cJSON_IsFalse(detached_item);
    arr_size_after = cJSON_GetArraySize(arr);

    // step 5: Configure/Integrate - add detached item back into root and inspect via object lookup
    cJSON_AddItemToObjectCS(root, "moved_item", detached_item);
    found_moved = cJSON_GetObjectItem(root, "moved_item");
    found_arr = cJSON_GetObjectItem(root, "items");
    cJSON_AddNumberToObject(root, "size_before", (double)arr_size_before);
    cJSON_AddNumberToObject(root, "size_after", (double)arr_size_after);
    cJSON_AddBoolToObject(root, "detached_was_false", detached_was_false);

    // step 6: Cleanup - free all allocated cJSON structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}