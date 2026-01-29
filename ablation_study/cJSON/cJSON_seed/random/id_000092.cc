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
//<ID> 92
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
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    const char *detached_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("alpha");
    item2 = cJSON_CreateString("beta");

    // step 3: Configure
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "flag", cJSON_CreateFalse());

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);
    detached_str = cJSON_GetStringValue(detached);
    cJSON_AddStringToObject(root, "detached_value", detached_str);

    // step 5: Validate and Cleanup
    cJSON_Delete(detached);
    cJSON_DeleteItemFromObject(root, "flag");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}