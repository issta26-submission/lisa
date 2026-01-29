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
//<ID> 48
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
    cJSON *first_item = cJSON_CreateTrue();
    cJSON *second_item = cJSON_CreateFalse();
    cJSON *detached_item = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    double size_diff = 0.0;
    cJSON_bool was_false = 0;

    // step 2: Setup - populate array and attach to root using case-sensitive keys
    cJSON_AddItemToArray(arr, first_item);
    cJSON_AddItemToArray(arr, second_item);
    cJSON_AddItemToObjectCS(root, "items", arr);

    // step 3: Operate - retrieve the array, measure size, detach an element, and detect its boolean state
    retrieved_array = cJSON_GetObjectItem(root, "items");
    size_before = cJSON_GetArraySize(retrieved_array);
    detached_item = cJSON_DetachItemFromArray(retrieved_array, 1);
    was_false = cJSON_IsFalse(detached_item);

    // step 4: Configure/Integrate - add the detached item back into the root and record metadata
    cJSON_AddItemToObjectCS(root, "moved_item", detached_item);
    cJSON_AddBoolToObject(root, "detached_was_false", was_false);
    size_after = cJSON_GetArraySize(retrieved_array);
    size_diff = (double)(size_before - size_after);
    cJSON_AddNumberToObject(root, "size_difference", size_diff);

    // step 5: Validate - fetch items to ensure they are available via object lookup
    cJSON_GetObjectItem(root, "moved_item");
    cJSON_GetObjectItem(root, "detached_was_false");

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}