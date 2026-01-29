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
//<ID> 441
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *config = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item_false = (cJSON *)0;
    cJSON *item_true = (cJSON *)0;
    cJSON *dup_config = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *got0 = (cJSON *)0;
    char *printed = (char *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool is_false_flag = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    config = cJSON_AddObjectToObject(root, "config");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "list", arr);
    item_false = cJSON_CreateFalse();
    item_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, item_false);
    cJSON_AddItemToArray(arr, item_true);

    // step 3: Configure
    size_before = cJSON_GetArraySize(arr);
    got0 = cJSON_GetArrayItem(arr, 0);
    is_false_flag = cJSON_IsFalse(got0);
    dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", dup_config);

    // step 4: Operate
    detached = cJSON_DetachItemViaPointer(arr, item_true);
    cJSON_AddItemToObject(root, "moved_item", detached);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    (void)size_before;
    (void)size_after;
    (void)is_false_flag;
    (void)got0;

    // step 6: Cleanup
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}