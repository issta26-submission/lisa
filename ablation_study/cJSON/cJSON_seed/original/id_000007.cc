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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_ref = cJSON_CreateStringReference("hello world");

    // step 2: Configure
    cJSON_AddItemToObject(child, "greeting", str_ref);
    cJSON *obj_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToArray(arr, child);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "child_ref", obj_ref);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "moved_child", detached);
    cJSON_DeleteItemFromObject(root, "array");
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *moved = cJSON_GetObjectItem(root, "moved_child");
    cJSON *greet = cJSON_GetObjectItem(moved, "greeting");
    const char *greet_val = cJSON_GetStringValue(greet);
    (void)greet_val;
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}