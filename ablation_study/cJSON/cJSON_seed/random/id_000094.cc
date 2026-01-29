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
//<ID> 94
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
    cJSON *arr2 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    const char *detached_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    arr2 = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("beta"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("gamma"));
    cJSON_AddItemToObject(root, "first_array", arr);
    cJSON_AddItemToObject(root, "second_array", arr2);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 1);
    detached_str = cJSON_GetStringValue(detached);
    cJSON_AddItemToArray(arr2, detached);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr2, false_item);

    // step 5: Validate
    cJSON_DeleteItemFromObject(root, "first_array");

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}