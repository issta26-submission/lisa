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
//<ID> 96
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *new_item = (cJSON *)0;
    const char *detached_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "removable", cJSON_CreateString("to_be_deleted"));
    cJSON_AddItemToObject(root, "flag", cJSON_CreateFalse());

    // step 3: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);
    detached_str = cJSON_GetStringValue(detached);
    new_item = cJSON_CreateString(detached_str);
    cJSON_AddItemToArray(arr, new_item);

    // step 4: Validate
    cJSON_DeleteItemFromObject(root, "removable");

    // step 5: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}