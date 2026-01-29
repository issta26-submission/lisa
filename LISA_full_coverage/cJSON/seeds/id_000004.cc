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
    // step 1: Declarations & Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *child = cJSON_CreateObject();
    cJSON *sref1 = cJSON_CreateStringReference("alpha");
    cJSON *sref2 = cJSON_CreateStringReference("beta");
    cJSON *objref = NULL;
    cJSON *detached = NULL;
    const char *det_val = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, sref1);
    cJSON_AddItemToArray(arr, sref2);
    cJSON_AddStringToObject(child, "name", "childnode");
    cJSON_AddItemToObject(root, "child_direct", child);
    objref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", objref);
    cJSON_AddTrueToObject(root, "to_remove");

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(arr, 0);
    det_val = cJSON_GetStringValue(detached);
    (void)det_val;
    cJSON_Delete(detached);
    cJSON_DeleteItemFromObject(root, "to_remove");

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}