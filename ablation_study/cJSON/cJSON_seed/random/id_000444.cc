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
//<ID> 444
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *dup_child = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool flag_is_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_AddObjectToObject(root, "child");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "items", arr);
    n0 = cJSON_CreateNumber(7.0);
    n1 = cJSON_CreateNumber(13.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 3: Configure
    size_before = cJSON_GetArraySize(arr);
    flag_is_false = cJSON_IsFalse(flag_false);

    // step 4: Operate
    dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_dup", dup_child);
    detached = cJSON_DetachItemViaPointer(child, arr);
    cJSON_AddItemToObject(root, "migrated_items", detached);
    size_after = cJSON_GetArraySize(detached);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    (void)size_before;
    (void)size_after;
    (void)flag_is_false;
    cJSON_free((void *)printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}