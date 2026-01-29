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
//<ID> 90
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
    cJSON *false_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    const char *first_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    first_item = cJSON_GetArrayItem(arr, 0);
    first_str = cJSON_GetStringValue(first_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);

    // step 5: Validate
    detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON_AddStringToObject(root, "first_value", first_str);
    cJSON_DeleteItemFromObject(root, "list");

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}