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
//<ID> 1432
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *str_item = NULL;
    cJSON *false_item = NULL;
    cJSON *detached = NULL;
    char *detached_str = NULL;
    cJSON_bool is_raw = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "my_array", arr);
    str_item = cJSON_CreateString("detachable");
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, false_item);

    // step 3: Operate / Validate
    is_raw = cJSON_IsRaw(str_item);
    cJSON_AddBoolToObject(root, "str_is_raw", is_raw);
    detached = cJSON_DetachItemViaPointer(arr, str_item);
    detached_str = cJSON_GetStringValue(detached);
    cJSON_AddItemToObject(root, "detached_copy", cJSON_CreateString(detached_str));

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);
    // API sequence test completed successfully
    return 66;
}