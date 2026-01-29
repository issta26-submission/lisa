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
//<ID> 447
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
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *dup_n0 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    char *printed = (char *)0;
    int arr_size = 0;
    cJSON_bool is_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(7.0);
    n1 = cJSON_CreateNumber(8.5);
    flag_false = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "values", arr);
    meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddItemToObject(meta, "name", cJSON_CreateString("tester"));
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    is_false = cJSON_IsFalse(flag_false);
    dup_n0 = cJSON_Duplicate(n0, 1);
    cJSON_AddItemToObject(root, "duplicate", dup_n0);
    detached = cJSON_DetachItemViaPointer(root, dup_n0);
    cJSON_AddItemToArray(arr, detached);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    (void)printed;
    (void)arr_size;
    (void)is_false;
    (void)detached;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}