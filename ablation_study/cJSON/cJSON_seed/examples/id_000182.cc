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
//<ID> 182
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
    cJSON_AddItemToObject(root, "arr", arr);
    const char *ref_str = "reference_value";
    cJSON *ref = cJSON_CreateStringReference(ref_str);
    cJSON_AddItemToObject(root, "ref", ref);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "child_key", cJSON_CreateString("child_val"));
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "child");
    char *printed_root = cJSON_PrintUnformatted(root);
    char *printed_detached = cJSON_PrintUnformatted(detached);

    // step 4: Validate & Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_AddItemToObject(root, "child_restored", detached);
    cJSON_Delete(root);

    return 66; // API sequence test completed successfully
}