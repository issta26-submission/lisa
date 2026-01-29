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
//<ID> 167
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
    cJSON *t1 = cJSON_CreateTrue();
    cJSON *f1 = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, t1);
    cJSON_AddItemToArray(arr, f1);

    // step 2: Configure
    cJSON *t_insert = cJSON_CreateTrue();
    cJSON_InsertItemInArray(arr, 1, t_insert);
    cJSON *container = cJSON_AddObjectToObject(root, "container");
    cJSON_AddItemToObject(container, "flags", arr);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_bool detached_is_bool = cJSON_IsBool(detached);
    cJSON *explicit_true = cJSON_AddTrueToObject(container, "explicit_true");
    cJSON_bool explicit_is_bool = cJSON_IsBool(explicit_true);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first);
    int summary = arr_size + (int)detached_is_bool + (int)explicit_is_bool + (int)first_is_bool + root->type + container->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}