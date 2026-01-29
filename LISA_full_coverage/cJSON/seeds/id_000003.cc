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
    // step 1: Declarations & Setup (Initialize → Configure)
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *sref1 = cJSON_CreateStringReference("alpha");
    cJSON *sref2 = cJSON_CreateStringReference("beta");
    cJSON_AddItemToArray(arr, sref1);
    cJSON_AddItemToArray(arr, sref2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *childObj = cJSON_CreateObject();
    cJSON *childStr = cJSON_CreateStringReference("childval");
    cJSON_AddItemToObject(childObj, "key", childStr);
    cJSON *objRef = cJSON_CreateObjectReference(childObj);
    cJSON_AddItemToObject(root, "childRef", objRef);
    cJSON_AddItemToObject(root, "own", cJSON_CreateStringReference("ownval"));

    // step 2: Operate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "detached", detached);

    // step 3: Validate
    cJSON *got = cJSON_GetObjectItem(root, "detached");
    const char *value = cJSON_GetStringValue(got);
    (void)value;
    (void)cJSON_IsString(got);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "own");
    cJSON_DeleteItemFromObject(root, "array");
    cJSON_DeleteItemFromObject(root, "detached");
    objRef->child = NULL;
    cJSON_Delete(childObj);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}