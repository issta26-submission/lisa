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
//<ID> 49
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
    cJSON *btrue = cJSON_CreateTrue();
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON *detached_item = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool was_false = 0;

    // step 2: Setup - populate array and attach to root
    cJSON_AddItemToArray(arr, btrue);
    cJSON_AddItemToArray(arr, bfalse);
    cJSON_AddItemToObjectCS(root, "items", arr);

    // step 3: Operate - retrieve array from root and measure size and detach item
    arr_ref = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(arr_ref);
    detached_item = cJSON_DetachItemFromArray(arr_ref, 1);

    // step 4: Configure - move detached item into root and record size
    cJSON_AddItemToObjectCS(root, "moved", detached_item);
    cJSON_AddNumberToObject(root, "size", (double)arr_size);

    // step 5: Validate - check if moved item is false and store flag
    was_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "moved"));
    cJSON_AddBoolToObject(root, "was_false", was_false);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}