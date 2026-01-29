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
//<ID> 95
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
    const char *detached_str = (const char *)0;
    cJSON *new_string = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");

    // step 3: Configure
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);
    detached_str = cJSON_GetStringValue(detached);
    new_string = cJSON_CreateString(detached_str);
    cJSON_AddItemToArray(arr, new_string);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 5: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "flag");
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}