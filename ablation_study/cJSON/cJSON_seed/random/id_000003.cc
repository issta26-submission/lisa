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
//<ID> 3
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
    cJSON *null_item = cJSON_CreateNull();
    cJSON *detached = NULL;

    // step 2: Setup - assemble array with one null element
    cJSON_AddItemToArray(arr, null_item);

    // step 3: Configure - create a reference to the array inside the root object
    cJSON_AddItemReferenceToObject(root, "array_ref", arr);

    // step 4: Operate - detach the item from the array and re-attach it (demonstrate detach/attach)
    detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToArray(arr, detached);

    // step 5: Validate & Cleanup preparations - remove the reference from the root object
    cJSON_DeleteItemFromObject(root, "array_ref");

    // step 6: Cleanup - delete created structures
    cJSON_Delete(arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}