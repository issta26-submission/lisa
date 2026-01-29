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
//<ID> 80
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
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "detachable");
    cJSON_AddItemToArray(arr, child);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "active");
    (void)flag;

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    cJSON *detached = cJSON_DetachItemViaPointer(arr, child);
    (void)arr_is_array;

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}