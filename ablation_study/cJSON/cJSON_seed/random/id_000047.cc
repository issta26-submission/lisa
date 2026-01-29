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
//<ID> 47
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
    cJSON *item_false = cJSON_CreateFalse();
    cJSON *item_str = cJSON_CreateString("alpha");
    cJSON *detached_item = (cJSON *)0;
    cJSON *found_item = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool was_false = 0;

    // step 2: Setup - populate array and attach to root (use case-sensitive keys)
    cJSON_AddItemToArray(arr, item_false);
    cJSON_AddItemToArray(arr, item_str);
    cJSON_AddItemToObjectCS(root, "items", arr);

    // step 3: Operate - detach the first element from the array
    detached_item = cJSON_DetachItemFromArray(arr, 0);

    // step 4: Configure - move detached item into root and inspect it
    cJSON_AddItemToObjectCS(root, "moved", detached_item);
    found_item = cJSON_GetObjectItem(root, "moved");
    was_false = cJSON_IsFalse(found_item);
    cJSON_AddBoolToObject(root, "moved_was_false", was_false);

    // step 5: Validate - record remaining array size into root
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "remaining", (double)arr_size);

    // step 6: Cleanup - delete the whole root (owns all added children)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}