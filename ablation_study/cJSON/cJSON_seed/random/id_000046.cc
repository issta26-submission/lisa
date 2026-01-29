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
//<ID> 46
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
    cJSON *b_true = cJSON_CreateTrue();
    cJSON *b_false = cJSON_CreateFalse();
    cJSON *detached = (cJSON *)0;
    cJSON *items_ref = (cJSON *)0;
    int size_after = 0;
    cJSON_bool was_false = 0;

    // step 2: Setup - build structure
    cJSON_AddItemToArray(arr, b_true);
    cJSON_AddItemToArray(arr, b_false);
    cJSON_AddItemToObjectCS(root, "items", arr);

    // step 3: Operate - detach second element and reattach to root
    detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObjectCS(root, "moved", detached);

    // step 4: Validate - inspect values and sizes
    was_false = cJSON_IsFalse(detached);
    items_ref = cJSON_GetObjectItem(root, "items");
    size_after = cJSON_GetArraySize(items_ref);

    // step 5: Cleanup - delete root (owns everything)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}