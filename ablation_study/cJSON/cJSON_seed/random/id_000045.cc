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
//<ID> 45
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
    cJSON *fetched_item = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool detached_is_false = 0;

    // step 2: Setup
    cJSON_AddItemToArray(arr, item_true);
    cJSON_AddItemToArray(arr, item_false);
    cJSON_AddItemToObjectCS(root, "list", arr);

    // step 3: Operate
    arr_size = cJSON_GetArraySize(arr);
    detached_item = cJSON_DetachItemFromArray(arr, 1);

    // step 4: Configure
    detached_is_false = cJSON_IsFalse(detached_item);
    cJSON_AddItemToObjectCS(root, "detached", detached_item);

    // step 5: Validate
    fetched_item = cJSON_GetObjectItem(root, "detached");
    (void)fetched_item;
    (void)detached_is_false;

    // step 6: Cleanup
    cJSON_AddNumberToObject(root, "size", (double)arr_size);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}