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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root_obj = cJSON_CreateObject();
    cJSON *root_array = cJSON_CreateArray();
    cJSON *null_array_item = cJSON_CreateNull();
    cJSON *null_obj_item = cJSON_CreateNull();
    cJSON *detached_item = (cJSON *)0;

    // step 2: Setup
    cJSON_AddItemToArray(root_array, null_array_item);
    cJSON_AddItemReferenceToObject(root_obj, "myref", null_obj_item);

    // step 3: Operate
    detached_item = cJSON_DetachItemFromArray(root_array, 0);

    // step 4: Validate
    cJSON_DeleteItemFromObject(root_obj, "myref");

    // step 5: Cleanup
    cJSON_Delete(detached_item);
    cJSON_Delete(root_array);
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}