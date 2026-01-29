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
//<ID> 81
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
    cJSON *node = cJSON_CreateObject();
    cJSON_AddStringToObject(node, "name", "node");
    cJSON_AddItemToArray(arr, node);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, arr);
    cJSON_bool detached_is_array = cJSON_IsArray(detached);
    char *printed_root = cJSON_Print(root);
    char *printed_detached = cJSON_Print(detached);
    int summary = (int)detached_is_array + (int)printed_root[0] + (int)printed_detached[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}