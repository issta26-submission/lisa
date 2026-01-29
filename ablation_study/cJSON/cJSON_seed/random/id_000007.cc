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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *detached = NULL;

    // step 2: Configure - build relationships
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemReferenceToObject(root, "refNull", null_item);

    // step 3: Operate - modify structures
    detached = cJSON_DetachItemFromArray(arr, 0);

    // step 4: Validate (logical flow) - remove the referenced item from object which will free it
    cJSON_DeleteItemFromObject(root, "refNull");

    // step 5: Cleanup - delete remaining root (which owns the array)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}