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
//<ID> 8
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
    cJSON *null_for_array = cJSON_CreateNull();
    cJSON *null_for_object = cJSON_CreateNull();
    cJSON *detached = NULL;
    cJSON_bool added_to_array = 0;
    cJSON_bool added_ref = 0;

    // step 2: Setup
    added_to_array = cJSON_AddItemToArray(arr, null_for_array);
    added_ref = cJSON_AddItemReferenceToObject(root, "my_null_ref", null_for_object);

    // step 3: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_Delete(detached);

    // step 4: Validate (remove the referenced item from the object)
    cJSON_DeleteItemFromObject(root, "my_null_ref");

    // step 5: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}