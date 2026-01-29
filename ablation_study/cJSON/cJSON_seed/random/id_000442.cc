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
//<ID> 442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *falseFlag = (cJSON *)0;
    int size = 0;
    cJSON_bool is_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_AddObjectToObject(root, "child");
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(7.0);
    n2 = cJSON_CreateNumber(13.0);
    falseFlag = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(child, "values", arr);
    cJSON_AddItemToObject(child, "flag", falseFlag);

    // step 4: Operate
    size = cJSON_GetArraySize(arr);
    dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "values_copy", dup_arr);
    is_false = cJSON_IsFalse(falseFlag);
    detached = cJSON_DetachItemViaPointer(root, child);

    // step 5: Validate
    (void)size;
    (void)is_false;
    (void)detached;
    (void)dup_arr;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}