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
//<ID> 1431
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
    cJSON *false_item = NULL;
    cJSON *str_item = NULL;
    cJSON *detached = NULL;
    const char *extracted = NULL;
    cJSON_bool is_raw = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    str_item = cJSON_CreateString("example_value");
    cJSON_AddItemToObject(root, "payload", arr);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Operate / Validate
    is_raw = cJSON_IsRaw(str_item);
    extracted = cJSON_GetStringValue(str_item);
    detached = cJSON_DetachItemViaPointer(arr, str_item);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}