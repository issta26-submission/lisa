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
//<ID> 213
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *t1 = cJSON_AddTrueToObject(child, "flag_a");
    cJSON *t2 = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "flag_b", t2);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("entry1"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("entry2"));

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "child");
    cJSON_bool is_obj = cJSON_IsObject(retrieved);
    cJSON_AddBoolToObject(root, "child_is_object", is_obj);
    cJSON *detached = cJSON_DetachItemViaPointer(child, t1);
    cJSON_AddItemToArray(arr, detached);
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", dup_child);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}