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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations and Initialization
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *nameStr = cJSON_CreateString("example");
    cJSON *temp = cJSON_CreateString("to_be_removed");
    cJSON_AddItemToObject(child, "name", nameStr);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure References and Arrays
    // Use a duplicate of `child` to avoid double-free when deleting `root`
    cJSON *objRef = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_ref", objRef);
    cJSON *strRef = cJSON_CreateStringReference("referenced_literal");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, strRef);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 3: Operate (detach from array and reattach) and cleanup a temp item
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON_DeleteItemFromObject(root, "temp");

    // step 4: Validate & Final Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}