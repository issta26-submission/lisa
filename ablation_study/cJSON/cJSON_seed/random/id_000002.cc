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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations (Initialize)
    cJSON *root = cJSON_CreateObject();
    cJSON *null_ref = cJSON_CreateNull();
    cJSON *arr = cJSON_CreateArray();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *detached = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool added_to_array = 0;

    // step 2: Configure
    added_ref = cJSON_AddItemReferenceToObject(root, "ref", null_ref);
    added_to_array = cJSON_AddItemToArray(arr, null_item);

    // step 3: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);

    // step 4: Validate
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Cleanup
    cJSON_DeleteItemFromObject(root, "ref");
    cJSON_Delete(detached);
    cJSON_Delete(arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}