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
//<ID> 445
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *objA = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    int size_before = 0;
    int size_after = 0;
    int size_dup = 0;
    cJSON_bool is_false_flag = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    objA = cJSON_AddObjectToObject(root, "config");
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(20.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "values", arr);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "feature_disabled", flag_false);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    dup_arr = cJSON_Duplicate(arr, 1);
    detached = cJSON_DetachItemViaPointer(arr, n1);
    size_after = cJSON_GetArraySize(arr);
    is_false_flag = cJSON_IsFalse(flag_false);
    dup_root = cJSON_Duplicate(root, 1);

    // step 5: Validate
    size_dup = cJSON_GetArraySize(dup_arr);
    (void)size_before;
    (void)size_after;
    (void)is_false_flag;
    (void)size_dup;
    (void)objA;
    (void)detached;
    (void)dup_root;

    // step 6: Cleanup
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_free((void *)printed);
    cJSON_Delete(detached);
    cJSON_Delete(dup_arr);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}