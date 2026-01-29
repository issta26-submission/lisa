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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *entry = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    double numeric_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Configure
    entry = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(entry, "val", 42.0);
    cJSON_AddItemToArray(arr, entry);
    str_item = cJSON_CreateString("detachable_string");
    cJSON_AddItemToArray(arr, str_item);

    // step 4: Operate
    is_array = cJSON_IsArray(arr);
    numeric_value = cJSON_GetNumberValue(cJSON_GetObjectItem(entry, "val"));

    // step 5: Validate / Modify
    detached = cJSON_DetachItemViaPointer(arr, str_item);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}