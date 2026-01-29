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
//<ID> 41
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
    cJSON *obj_items = (cJSON *)0;
    cJSON *obj_detached = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool was_false = 0;
    cJSON_bool confirm_false = 0;
    double meta_value = 0.0;

    // step 2: Setup (initialize structure and attach array to root)
    cJSON_AddItemToArray(arr, item_true);
    cJSON_AddItemToArray(arr, item_false);
    cJSON_AddItemToObjectCS(root, "items", arr);

    // step 3: Operate (inspect array, detach an item, and reattach it under root)
    obj_items = cJSON_GetObjectItem(root, "items");
    size_before = cJSON_GetArraySize(obj_items);
    detached_item = cJSON_DetachItemFromArray(obj_items, 1);
    was_false = cJSON_IsFalse(detached_item);
    cJSON_AddItemToObjectCS(root, "detached", detached_item);
    size_after = cJSON_GetArraySize(obj_items);

    // step 4: Validate (retrieve detached object and record meta information)
    obj_detached = cJSON_GetObjectItem(root, "detached");
    confirm_false = cJSON_IsFalse(obj_detached);
    meta_value = (double)size_before + (double)size_after + (double)was_false + (double)confirm_false;
    cJSON_AddNumberToObject(root, "meta", meta_value);

    // step 5: Cleanup (free all allocated cJSON structures)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}