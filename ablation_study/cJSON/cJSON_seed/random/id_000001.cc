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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations (Initialize)
    cJSON *array = cJSON_CreateArray();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *detached = NULL;
    cJSON *obj = cJSON_CreateObject();
    cJSON_bool ref_added = 0;

    // step 2: Setup (Configure)
    cJSON_AddItemToArray(array, null_item);

    // step 3: Core operations (Operate)
    detached = cJSON_DetachItemFromArray(array, 0);
    ref_added = cJSON_AddItemReferenceToObject(obj, "ref", detached);

    // step 4: Validate (use operations' effects) 
    /* No branches allowed; validation is performed by exercising APIs that act on produced state */
    cJSON_DeleteItemFromObject(obj, "ref");

    // step 5: Cleanup
    cJSON_Delete(array);
    cJSON_Delete(obj);

    // API sequence test completed successfully
    (void)ref_added;
    return 66;
}