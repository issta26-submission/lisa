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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *childObj = cJSON_CreateObject();
    cJSON *child_ref = NULL;
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *s3 = NULL;
    cJSON *detached = NULL;

    // step 2: Setup
    cJSON_AddItemToObject(childObj, "inner", cJSON_CreateString("inner_value"));
    cJSON_AddItemToObject(root, "child", childObj);
    child_ref = cJSON_CreateObjectReference(childObj);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    s3 = cJSON_CreateStringReference("gamma");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 3: Core operations
    detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObject(childObj, "moved", detached);
    cJSON_DeleteItemFromObject(root, "child_ref");

    // step 4: Cleanup and validation
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}