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
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "key", "value");

    // step 2: Configure
    cJSON *child_dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToArray(arr, child_dup);
    cJSON *sref = cJSON_CreateStringReference("hello");
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "childRef", child_ref);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_DeleteItemFromObject(root, "childRef");

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(child);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}