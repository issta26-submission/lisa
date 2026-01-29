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
//<ID> 448
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
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *computed = (cJSON *)0;
    cJSON *dupe = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    int sz = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_AddObjectToObject(root, "child");
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(7.0);
    n1 = cJSON_CreateNumber(13.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(child, "arr", arr);
    flag_false = cJSON_AddFalseToObject(child, "flag");

    // step 4: Operate
    sz = cJSON_GetArraySize(arr);
    computed = cJSON_CreateNumber((double)sz + 0.5);
    cJSON_AddItemToObject(root, "computed", computed);
    dupe = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", dupe);
    detached = cJSON_DetachItemViaPointer(root, child);
    (void)cJSON_IsFalse(flag_false);
    sz = cJSON_GetArraySize(arr);

    // step 5: Validate
    (void)sz;
    (void)dupe;
    (void)detached;

    // step 6: Cleanup
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}