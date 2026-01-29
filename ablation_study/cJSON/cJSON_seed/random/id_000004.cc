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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *nullItem = cJSON_CreateNull();
    cJSON *detached = NULL;
    cJSON_bool added_to_array = 0;
    cJSON_bool added_ref = 0;

    // step 2: Setup - populate array
    added_to_array = cJSON_AddItemToArray(array, nullItem);

    // step 3: Configure - add a reference of the array into the root object
    added_ref = cJSON_AddItemReferenceToObject(root, "arr", array);

    // step 4: Operate - detach the first item from the array
    detached = cJSON_DetachItemFromArray(array, 0);

    // step 5: Validate / Cleanup operations - remove the reference from the object and free resources
    cJSON_DeleteItemFromObject(root, "arr");
    cJSON_Delete(detached);
    cJSON_Delete(array);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}