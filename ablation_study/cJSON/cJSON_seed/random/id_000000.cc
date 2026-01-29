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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *null_item = NULL;
    cJSON *detached = NULL;
    cJSON *retrieved = NULL;
    int arr_size = 0;
    cJSON_bool added_to_array = 0;
    cJSON_bool added_ref_to_object = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    null_item = cJSON_CreateNull();

    // step 3: Operate
    added_to_array = cJSON_AddItemToArray(arr, null_item);
    detached = cJSON_DetachItemFromArray(arr, 0);

    // step 4: Configure -> add reference into object and validate presence
    added_ref_to_object = cJSON_AddItemReferenceToObject(root, "myNull", detached);
    retrieved = cJSON_GetObjectItem(root, "myNull");
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Cleanup (remove from object and delete remaining structures)
    cJSON_DeleteItemFromObject(root, "myNull");
    cJSON_Delete(arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}