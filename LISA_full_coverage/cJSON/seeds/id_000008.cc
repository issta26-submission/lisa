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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    const char *greeting = "hello world";
    cJSON *strRef = cJSON_CreateStringReference(greeting);
    cJSON *childRef = cJSON_CreateObjectReference(child);
    cJSON *detached = NULL;
    cJSON *dup = NULL;
    cJSON_bool compare_result = 0;

    // step 2: Configure & Operate
    cJSON_AddItemToObject(child, "greeting", strRef);
    cJSON_AddItemToArray(arr, childRef);
    detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON_DeleteItemFromObject(child, "greeting");

    // step 3: Validate
    dup = cJSON_Duplicate(root, 1);
    compare_result = cJSON_Compare(root, dup, 1);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    cJSON_Delete(arr);
    cJSON_Delete(child);

    // API sequence test completed successfully
    (void)compare_result;
    return 66;
}