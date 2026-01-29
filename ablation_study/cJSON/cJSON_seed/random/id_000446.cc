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
//<ID> 446
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
    cJSON *num = (cJSON *)0;
    cJSON *config = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON_bool false_flag_check = 0;
    int original_size = 0;
    int dup_size_before = 0;
    int dup_size_after = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "values", arr);
    config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddItemToObject(config, "enabled", cJSON_CreateFalse());

    // step 3: Configure
    dup = cJSON_Duplicate(arr, 1);
    dup_size_before = cJSON_GetArraySize(dup);

    // step 4: Operate
    first = cJSON_GetArrayItem(dup, 0);
    detached = cJSON_DetachItemViaPointer(dup, first);
    dup_size_after = cJSON_GetArraySize(dup);
    false_flag_check = cJSON_IsFalse(cJSON_GetObjectItem(config, "enabled"));
    original_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    (void)dup_size_before;
    (void)dup_size_after;
    (void)original_size;
    (void)false_flag_check;
    (void)detached;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}