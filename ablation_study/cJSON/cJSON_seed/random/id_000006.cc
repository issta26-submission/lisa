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
//<ID> 6
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
    cJSON_bool added_to_array = 0;
    cJSON_bool added_ref_to_object = 0;

    // step 2: Setup
    added_to_array = cJSON_AddItemToArray(arr, null_item);

    // step 3: Configure
    added_ref_to_object = cJSON_AddItemReferenceToObject(root, "myArray", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);

    // step 5: Validate / Cleanup operations
    cJSON_DeleteItemFromObject(root, "myArray");
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}