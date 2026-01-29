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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations (Initialize)
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *null1 = cJSON_CreateNull();
    cJSON *null2 = cJSON_CreateNull();
    cJSON *detached1 = nullptr;
    cJSON *detached2 = nullptr;

    // step 2: Setup (Configure)
    cJSON_AddItemToObject(root, "arr", array);
    cJSON_AddItemToArray(array, null1);
    cJSON_AddItemToArray(array, null2);
    cJSON_AddItemReferenceToObject(root, "a_null", null1);

    // step 3: Operate
    detached1 = cJSON_DetachItemFromArray(array, 0);

    // step 4: Validate
    cJSON_DeleteItemFromObject(root, "a_null");

    // step 5: Cleanup
    detached2 = cJSON_DetachItemFromArray(array, 0);
    cJSON_Delete(detached2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}