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
//<ID> 449
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *subobj = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *s0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    int sz = 0;
    cJSON_bool is_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("entry");
    n1 = cJSON_CreateNumber(2025.0);
    flag_false = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "values", arr);
    subobj = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddItemToObject(subobj, "enabled", flag_false);

    // step 4: Operate
    sz = cJSON_GetArraySize(arr);
    is_false = cJSON_IsFalse(flag_false);
    dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "values_copy", dup_arr);
    detached = cJSON_DetachItemViaPointer(root, dup_arr);

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)sz;
    (void)is_false;
    (void)detached;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}